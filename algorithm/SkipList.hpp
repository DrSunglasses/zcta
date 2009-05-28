/* 
 * File:   SkipList.hpp
 * Author: Jeffrey Bosboom
 *
 * Created on April 25, 2009, 2:55 PM
 */

#ifndef _SKIPLIST_HPP
#define	_SKIPLIST_HPP

#include <stdexcept>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/binomial_distribution.hpp>
#include "../memory/FixedAllocator.hpp"

using boost::shared_ptr;
using boost::weak_ptr;

template<class T>
struct LessComparator {
	int operator()(const T& a, const T& b) {
		if (std::less<T>()(a, b)) //a < b
			return -1;
		if (std::less<T>()(b, a)) //b > a
			return 1;
		return 0; //a == b
	}
};

template<typename T, typename Cmp = LessComparator<T> >
class SkipList : boost::noncopyable {
private:
	//forward declarations
	class Node;
	class ValueNode;
	class NegInfNode;
	class PosInfNode;
	class Iter;
	//member variables
	Node* topLeft_;
	int size_, height_;
	const shared_ptr<Cmp> comparator;
	boost::variate_generator<boost::mt19937, boost::bernoulli_distribution<> > random_generator;
	FixedAllocator nodeAlloc, valueNodeAlloc;
public:
	typedef Iter iterator;
	//constructor
	explicit SkipList() : size_(0), height_(1), comparator(new Cmp()),
			random_generator(boost::mt19937(), boost::bernoulli_distribution<>()),
			nodeAlloc(sizeof(Node), 255), valueNodeAlloc(sizeof(ValueNode), 255) {
		init();
	}
	explicit SkipList(shared_ptr<Cmp> pCmp) : size_(0), height_(1), comparator(pCmp),
			random_generator(boost::mt19937(), boost::bernoulli_distribution<>()),
			nodeAlloc(sizeof(Node), 255), valueNodeAlloc(sizeof(ValueNode), 255) {
		init();
	}
	//destructor
	~SkipList() { //delete all the nodes
		Node* cur = topLeft_;
		Node* nextLevel = cur->down;
		while (nextLevel != 0) {
			nextLevel = cur->down;
			while (cur != 0) {
				Node* next = cur->next;
				if (cur->isPosInf() || cur->isNegInf())
					delete cur;
				else
					deleteNode(cur);
				cur = next;
			}
			cur = nextLevel;
		}
	}
	//interface functions
	int size() const {
		return size_;
	}
	bool empty() const {
		return size_ == 0;
	}
	int height() const {
		return height_;
	}
	iterator begin() const {
		Node* start = topLeft_;
		while (start->down)
			start = start->down;
		return Iter(start->next->getNodePtr());
	}
	iterator end() const {
		Node* start = topLeft_;
		while (start->next)
			start = start->next;
		return Iter(start->getNodePtr());
	}
	iterator find(const T& k) {
		Node* n = skipSearch(k);
		if (n->isNegInf() || n->isPosInf())
			return end();
		if (compare(k, n->get()) == 0)
			return Iter(n->getNodePtr());
		return end();
	}
	iterator add(const T& k) {
		Node* p = skipSearch(k);
		Node* e = newValueNode(k);
		Node* q = insertAfterAbove(p, 0, e);
		int i = 0;
		while (random_generator()) {
			i++;
			if (i >= height_) {
				height_++;
				Node* t = topLeft_->next;
				topLeft_ = insertAfterAbove(0, topLeft_, new NegInfNode());
				insertAfterAbove(topLeft_, t, new PosInfNode());
			}
			while (!p->up)
				p = p->prev;
			p = p->up;
			q = insertAfterAbove(p, q, newNode());
		}
		size_++;
		return iterator(e->getNodePtr());
	}
	void remove(const iterator& i) {
		if (!i.valid())
			throw std::logic_error("Remove on invalid iterator");
		Node* n = i.node(), *oldn;
		while (n) {
			n->prev->next = n->next;
			n->next->prev = n->prev;
			oldn = n;
			n = n->up;
			deleteNode(oldn);
		}
		size_--;
	}
	void swap(const iterator& left, const iterator& right) {
		if (left.hasPrev() && left.prev() == right)
			return swap(right, left);
		if (left.next() != right)
			throw std::logic_error("Swapping non-adjacent elements");
		Node* lnode = left.node();
		Node* rnode = right.node();
		Node* ll = lnode->prev;
		Node* rr = rnode->next;
		lnode->next = rr;
		rr->prev = lnode;
		rnode->prev = ll;
		ll->next = rnode;
		lnode->prev = rnode;
		rnode->next = lnode;
		Node* rup = rnode->up;
		Node* lup = lnode->up;
		if (lup) {
			rnode->up = lup;
			lup->down = rnode;
		} else
			rnode->up = 0;
		if (rup) {
			lnode->up = rup;
			rup->down = lnode;
		} else
			lnode->up = 0;
	}
	shared_ptr<Cmp> getComparator() {
		return comparator;
	}
private:
	//implementation functions
	Node* skipSearch(T key) {
		Node* p = topLeft_;
        while (p->down != 0) {
            p = p->down;
            while (!p->next->isPosInf() && compare(key, p->next->get()) >= 0)
                p = p->next;
        }
        return p;
	}
	Node* insertAfterAbove(Node* before, Node* below, Node* toInsert) {
		Node* after = 0;
        if (before) {
            after = before->next;
            before->next = toInsert;
        }
        if (after)
            after->prev = toInsert;
        toInsert->next = after;
        if (below)
            below->up = toInsert;
        toInsert->prev = before;
        toInsert->down = below;
		return toInsert;
	}
	void init() {
		topLeft_ = new NegInfNode();
		topLeft_->down = new NegInfNode();
		topLeft_->down->up = topLeft_;
		topLeft_->next = new PosInfNode();
		topLeft_->next->prev = topLeft_;
		topLeft_->next->down = new PosInfNode();
		topLeft_->next->down->up = topLeft_->next;
		topLeft_->down->next = topLeft_->next->down;
		topLeft_->down->next->prev = topLeft_->down->next;
	}
	int compare(T a, T b) {
		return (*comparator)(a, b);
	}
	Node* newNode() {
		void* ptr = nodeAlloc.allocate();
		return new (ptr) Node();
	}
	ValueNode* newValueNode(T val) {
		void* ptr = valueNodeAlloc.allocate();
		return new (ptr) ValueNode(val);
	}
	void deleteNode(Node* ptr) {
		if (!ptr)
			throw std::logic_error("Deleting null Node");
		if (ptr->isPosInf())
			throw std::logic_error("Deleting a PosInfNode");
		if (ptr->isNegInf())
			throw std::logic_error("Deleting a NegInfNode");
		if (!ptr->isValueNode()) {
			ptr->~Node();
			nodeAlloc.deallocate(ptr);
		} else {
			ptr->~Node(); //virtual destructor, calls right one
			valueNodeAlloc.deallocate(ptr);
		}
	}
	//implementation classes
	struct Node {
		Node *up, *down, *prev, *next;
		Node() : up(0), down(0), prev(0), next(0) {}
		virtual T& get() {
			Node* foo = down;
			while (foo->down != 0)
				foo = foo->down;
			return foo->get();
		}
		virtual bool isNegInf() const { return false; }
		virtual bool isPosInf() const { return false; }
		virtual bool isValueNode() const { return false; }
		virtual weak_ptr<Node> getNodePtr() const {
			const Node* foo = this;
			while (foo->down != 0)
				foo = foo->down;
			return foo->getNodePtr();
		}
		virtual ~Node() {} //make sure deletion works properly
	};
	class NullDeleter;
	struct ValueNode : public Node {
		explicit ValueNode(T val) : selfReference(this, NullDeleter()), value(val) {}
		virtual T& get() {
			return value;
		}
		virtual weak_ptr<Node> getNodePtr() const {
			return selfReference; //uses conversion on return
		}
		virtual bool isValueNode() const { return true; }
		shared_ptr<ValueNode> selfReference;
		T value;
	private:
		ValueNode(const ValueNode& other);
		ValueNode& operator=(const ValueNode& other);
	};
	struct NegInfNode : public Node {
		virtual T& get() {
			throw std::logic_error("get() on NegInfNode");
		}
		virtual bool isNegInf() const { return true; }
		virtual weak_ptr<Node> getNodePtr() const {
			return weak_ptr<Node>(); //uses conversion on return
		}
	};
	struct PosInfNode : public Node {
		virtual T& get() {
			throw std::logic_error("get() on PosInfNode");
		}
		virtual bool isPosInf() const { return true; }
		virtual weak_ptr<Node> getNodePtr() const {
			return weak_ptr<Node>(); //uses conversion on return
		}
	};
	class Iter {
	private:
		weak_ptr<Node> cur;
	public:
		typedef T value_type;
		Iter() : cur(0) {}
		Iter(const weak_ptr<Node>& node) : cur(node) {}
		Iter(const Iter& other) : cur(other.cur) {}
		iterator& operator=(const iterator& other) {
			cur = other.cur;
			return *this;
		}
		value_type& operator*() const {
			if (!valid())
				throw std::logic_error("dereferencing invalid iterator");
			shared_ptr<Node> a = cur.lock();
			return a->get();
		}
		value_type* operator->() const {
			if (!valid())
				throw std::logic_error("arrowing invalid iterator");
			shared_ptr<Node> a = cur.lock();
			return &(a->get());
		}
		bool valid() const {
			return !cur.expired();
		}
		bool hasNext() const {
			if (!valid())
				throw std::logic_error("hasNext on invalid iterator");
			shared_ptr<Node> a = cur.lock();
			return a->next && !a->next->isPosInf();
		}
		bool hasPrev() const {
			if (!valid())
				throw std::logic_error("hasPrev on invalid iterator");
			shared_ptr<Node> a = cur.lock();
			return a->prev && !a->prev->isNegInf();
		}
		iterator prev() const {
			if (!valid())
				throw std::logic_error("prev() on invalid iterator");
//			if (!hasPrev())
//				throw std::logic_error("prev() when hasPrev() == false");
			return iterator(cur.lock()->prev->getNodePtr());
		}
		iterator next() const {
			if (!valid())
				throw std::logic_error("next() on invalid iterator");
//			if (!hasNext())
//				throw std::logic_error("next() when hasNext() == false");
			return iterator(cur.lock()->next->getNodePtr());
		}
		iterator& operator++() {
			if (!valid())
				throw std::logic_error("++ on invalid iterator");
			shared_ptr<Node> a = cur.lock();
			cur = a->next->getNodePtr();
			return *this;
		}
		iterator operator++(int) {
			Iter copy = *this;
			++(*this);
			return copy;
		}
		iterator& operator--() {
			if (!valid())
				throw std::logic_error("-- on invalid iterator");
			shared_ptr<Node> a = cur.lock();
			cur = a->prev->getNodePtr();
			return *this;
		}
		iterator operator--(int) {
			Iter copy = *this;
			--(*this);
			return copy;
		}
		bool operator==(const iterator& other) const {
			if (!valid())
				return !other.valid();
			shared_ptr<Node> us = cur.lock();
			shared_ptr<Node> them = other.cur.lock();
			return us == them;
		}
		bool operator!=(const iterator& other) const {
			return !(*this == other);
		}
	private:
		Node* node() const {
			return cur.lock().get();
		}
		friend class SkipList;
	};
	struct NullDeleter {
		void operator()(const void*) {}
	};
};

#endif	/* _SKIPLIST_HPP */

