/* 
 * File:   Algorithm.hpp
 * Author: Jeffrey Bosboom
 *
 * Created on May 11, 2009, 7:51 PM
 */

#ifndef _ALGORITHM_HPP
#define	_ALGORITHM_HPP

#include <queue>
#include <typeinfo>
#include <boost/lexical_cast.hpp>
#include "SkipList.hpp"
#include "Event.hpp"
#include "AdjacencySet.hpp"

template<typename Target, typename Source>
Target* throwing_dynamic_cast(Source* src) {
	Target* ret = dynamic_cast<Target*>(src);
	if (ret)
		return ret;
	throw std::bad_cast();
}

//struct LineComparator {
//	int operator()(const Line& a, const Line& b);
//	void update(const Point& pt);
//	bool pointInFuture(const Point& pt);
//	bool parallelToSweepLine(const ZCTASegment& seg);
//}

template<class PointComparator, class LineComparator>
class Algorithm {
	//private types
	typedef typename SkipList<ZCTALine, LineComparator>::iterator Position;
	struct EventComparator {
		//returns true if a < b (that is, a occurs before b)
		bool operator()(const Event<LineComparator>* const a, const Event<LineComparator>* const b) {
			int res = PointComparator()(a->eventPoint(), b->eventPoint());
			if (res < 0)
				return true;
			if (res > 0)
				return false;
			if (a->priority() < b->priority())
				return true;
			if (a->priority() > b->priority())
				return false;
			//priority_queue requires a strict weak ordering, so compare pointers
			if (a < b)
				return true;
			if (a > b)
				return false;
			return false;
		}
	};
	template<class Comparator, class T>
	struct ReversingComparator {
		bool operator()(T a, T b) {
			return Comparator()(a, b) ? false : true;
		}
	};
	//instance data
	AdjacencySet* adjacencies;
	double adjacencyTolerance;
	boost::shared_ptr<LineComparator> lineCmp;
	std::priority_queue<Event<LineComparator>*, std::vector<Event<LineComparator>*>, ReversingComparator<EventComparator, const Event<LineComparator>* const> > eventQueue;
	SkipList<ZCTALine, LineComparator> sweepLine;
	boost::unordered_set<SwapEvent<LineComparator> > processedEvents;
public:
	Algorithm(const boost::unordered_set<ZCTASegment>& segments, double tolerance, AdjacencySet* results)
			:  adjacencies(results), adjacencyTolerance(tolerance), lineCmp(new LineComparator()), eventQueue(),
			sweepLine(lineCmp), processedEvents() {
		for (boost::unordered_set<ZCTASegment>::const_iterator i = segments.begin(); i != segments.end(); ++i) {
			const ZCTASegment& seg = *i;
			const Point& a = seg.a(), &b = seg.b();
			eventQueue.push(new AddEvent<LineComparator>(seg, PointComparator()(a, b) < 0 ? a : b));
		}
	}
	//actual work is done here
	void operator()() {
		while (!eventQueue.empty()) {
			Event<LineComparator>* evt = eventQueue.top();
			eventQueue.pop();
			lineCmp->update(evt->eventPoint());
			switch (evt->type()) {
				case ADD:
					add(evt);
					break;
				case DELETE:
					remove(evt);
					break;
				case SWAP:
					swap(evt);
					break;
				default:
					throw std::logic_error("Unknown event type: " + boost::lexical_cast<std::string>(evt->type()));
			}
			delete evt;
		}
	}
private:
	void add(Event<LineComparator>* parameter) {
		AddEvent<LineComparator>* evt = throwing_dynamic_cast<AddEvent<LineComparator> >(parameter);
		const ZCTASegment& seg = evt->segment();
		if (lineCmp->parallelToSweepLine(seg)) {
			const ZCTA& us = seg.zcta();
			for (Position p = sweepLine.begin(); p != sweepLine.end(); ++p)
				for (ZCTALine::iterator a = p->begin(); a != p->end(); ++a) {
					const ZCTA& them = a->zcta();
					if (us != them && !adjacencies->areAdjacent(us, them) && seg.distanceTo(*a) < adjacencyTolerance)
						adjacencies->addAdjacency(us, them);
				}
		} else {
			const Line& line = seg.line();
			ZCTALine zLine(line);
			Position pos = sweepLine.find(zLine);
			if (pos != sweepLine.end())
				pos->add(seg);
			else {
				zLine.add(seg);
				pos = sweepLine.add(zLine);
			}
			eventQueue.push(new DeleteEvent<LineComparator>(seg, pos, endPoint(seg)));
			if (pos.hasPrev())
				compare(pos.prev(), pos);
			if (pos.hasNext())
				compare(pos, pos.next());
		}
	}
	void remove(Event<LineComparator>* parameter) {
		DeleteEvent<LineComparator>* evt = throwing_dynamic_cast<DeleteEvent<LineComparator> >(parameter);
		const Position& pos = evt->position();
		if (!pos.valid())
			throw std::logic_error("Removed a line with active segments");
		if (pos.hasPrev())
			compare(pos.prev(), pos);
		if (pos.hasNext())
			compare(pos, pos.next());
		pos->remove(evt->segment());
		if (pos->empty()) {
			Position prev = pos.prev(), next = pos.next();
			sweepLine.remove(pos);
			if (prev.valid() && next.valid())
				compare(prev, next);
		}
	}
	void swap(Event<LineComparator>* parameter) {
		SwapEvent<LineComparator>* evt = throwing_dynamic_cast<SwapEvent<LineComparator> >(parameter);
		if (!evt->posA().valid() || !evt->posB().valid())
			return;
		if (processedEvents.find(*evt) != processedEvents.end())
			return;
		const Position &a = evt->posA(), &b = evt->posB();
		if (a.next() != b) {
//			std::cout << parameter->eventPoint() << std::endl;
//			for (int i = 0; i < 5; i++) {
//				std::cout << boost::lexical_cast<std::string>(eventQueue.top()->type())
//						<< " " << eventQueue.top()->eventPoint() << std::endl;
//				eventQueue.pop();
//			}
			throw std::logic_error("Swapping non-adjacent lines");
		}
		Position aNeighbor = a.prev(), bNeighbor = b.next();
		sweepLine.swap(a, b);
		if (aNeighbor.valid())
			compare(aNeighbor, b);
		if (bNeighbor.valid())
			compare(a, bNeighbor);
		processedEvents.insert(*evt);
	}
	void compare(Position left, Position right) {
		if (left.next() != right)
			throw std::logic_error("Comparing non-adjacent iterators");
		if (left == right)
			throw std::logic_error("Comparing a line against itself");
		if (*left == *right)
			throw std::logic_error("Comparing a line against itself via two of the same line in list!");
		for (ZCTALine::iterator li = left->begin(); li != left->end(); ++li)
			for (ZCTALine::iterator ri = right->begin(); ri != right->end(); ++ri) {
				const ZCTA &lz = li->zcta(), &rz = ri->zcta();
				if (lz != rz && !adjacencies->areAdjacent(lz, rz) && li->distanceTo(*ri) < adjacencyTolerance)
					adjacencies->addAdjacency(lz, rz);
			}
		if (left->parallelTo(*right))
			return;
		Point pt = left->intersectionWith(*right);
		if (lineCmp->pointInFuture(pt))
			eventQueue.push(new SwapEvent<LineComparator>(*left, *right, left, right, pt));
	}
	const Point& startPoint(const LineSegment& seg) {
		int res = PointComparator()(seg.a(), seg.b());
		if (res < 0)
			return seg.a();
		else if (res > 0)
			return seg.b();
		throw std::logic_error("Segment without start point");
	}
	const Point& endPoint(const LineSegment& seg) {
		int res = PointComparator()(seg.a(), seg.b());
		if (res > 0)
			return seg.a();
		else if (res < 0)
			return seg.b();
		throw std::logic_error("Segment without end point");
	}
};

#endif	/* _ALGORITHM_HPP */

