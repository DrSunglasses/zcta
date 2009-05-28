/* 
 * File:   Event.hpp
 * Author: Jeffrey Bosboom
 *
 * Created on May 11, 2009, 9:11 PM
 */

#ifndef _EVENT_HPP
#define	_EVENT_HPP

#include "../math/Point.hpp"
#include "ZCTALine.hpp"
#include "SkipList.hpp"
#include "ZCTASegment.hpp"

enum EventType {ADD, DELETE, SWAP};

template<class SkipListCmp>
class Event {
public:
	virtual const Point& eventPoint() const = 0;
	/**
	 * Current priority assignments:
	 * AddEvent -- 30
	 * SwapEvent -- 20
	 * DeleteEvent -- 10
	 *
	 * Lower numbers represent higher priority.
	 */
	virtual int priority() const = 0;
	virtual EventType type() const = 0;
	virtual ~Event() {};
};

template<class SkipListCmp>
class AddEvent : public Event<SkipListCmp> {
	ZCTASegment seg;
	Point evtPt;
public:
	AddEvent(const ZCTASegment& segment, const Point& eventPoint) : seg(segment), evtPt(eventPoint) {
	}
	virtual const Point& eventPoint() const {
		return evtPt;
	}
	virtual int priority() const {
		return 30;
	}
	virtual EventType type() const {
		return ADD;
	}
	const ZCTASegment& segment() const {
		return seg;
	}
};

template<class SkipListCmp>
class DeleteEvent : public Event<SkipListCmp> {
	ZCTASegment seg;
	typename SkipList<ZCTALine, SkipListCmp>::iterator pos;
	Point evtPt;
public:
	DeleteEvent(const ZCTASegment& segment, const typename SkipList<ZCTALine, SkipListCmp>::iterator& position,
			const Point& eventPoint)
			: seg(segment), pos(position), evtPt(eventPoint) {
	}
	virtual const Point& eventPoint() const {
		return evtPt;
	}
	virtual int priority() const {
		return 10;
	}
	virtual EventType type() const {
		return DELETE;
	}
	const ZCTASegment& segment() const {
		return seg;
	}
	typename SkipList<ZCTALine, SkipListCmp>::iterator position() const {
		return pos;
	}
};

template<class SkipListCmp>
class SwapEvent : public Event<SkipListCmp> {
	ZCTALine a, b;
	typename SkipList<ZCTALine, SkipListCmp>::iterator pos_a, pos_b;
	Point evtPt;
public:
	SwapEvent(const ZCTALine& segA, const ZCTALine& segB, const typename SkipList<ZCTALine, SkipListCmp>::iterator& posA,
			const typename SkipList<ZCTALine, SkipListCmp>::iterator& posB, const Point& eventPoint)
			: a(segA), b(segB), pos_a(posA), pos_b(posB), evtPt(eventPoint) {
	}
	virtual const Point& eventPoint() const {
		return evtPt;
	}
	virtual int priority() const {
		return 20;
	}
	virtual EventType type() const {
		return SWAP;
	}
	typename SkipList<ZCTALine, SkipListCmp>::iterator posA() const {
		return pos_a;
	}
	typename SkipList<ZCTALine, SkipListCmp>::iterator posB() const {
		return pos_b;
	}
	bool operator==(const SwapEvent& evt) const {
		return a == evt.a && b == evt.b && pos_a == evt.pos_a && pos_b == evt.pos_b && evtPt == evt.evtPt;
	}

	friend std::size_t hash_value(const SwapEvent& evt) {
		std::size_t seed = 0;
		boost::hash_combine(seed, evt.a);
		boost::hash_combine(seed, evt.b);
		return seed;
	}
};

#endif	/* _EVENT_HPP */

