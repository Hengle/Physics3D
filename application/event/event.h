#pragma once

#include <string>

enum class EventType {
	None = 0,
	WindowClose, WindowResize, FrameBufferResize,
	MouseScroll, MouseMove, MousePress, MouseRelease, MouseExit, MouseEnter,
	KeyPress, KeyRelease, KeyDoublePress
};

enum EventCategory {
	EventCategoryNone		 = 0 << 0,
	EventCategoryInput		 = 1 << 0,
	EventCategoryMouse		 = 1 << 1, 
	EventCategoryMouseButton = 1 << 2, 
	EventCategoryKeyboard	 = 1 << 3,
	EventCategoryWindow		 = 1 << 4,
	EventCategoryApplication = 1 << 5
};

#define EVENT_TYPE(type) \
	inline static EventType getStaticType() { return EventType::type; } \
	inline virtual EventType getType() const override { return getStaticType(); } \
	inline virtual std::string getName() const override { return #type; }

#define EVENT_CATEGORY(category) \
	virtual char getCategory() const override { return category; }

class Event {
public:
	bool handled = false;

	inline virtual EventType getType() const = 0;	
	inline virtual char getCategory() const = 0;
	inline virtual std::string getName() const = 0;

	inline bool inCategory(EventCategory category) const {
		return static_cast<char>(category) == getCategory();
	}

};

#define BIND_EVENT(function) \
	std::bind(&function, this, std::placeholders::_1)

#define DISPATCH_EVENT(event, type, function) \
	{ EventDispatcher dispatch(event); \
	dispatch.dispatch<type>(BIND_EVENT(function)); }

class EventDispatcher {
private:
	Event& event;
public:
	EventDispatcher(Event& event) : event(event) {}

	template<typename T, typename F>
	bool dispatch(const F& function) {
		if (event.getType() == T::getStaticType()) {
			event.handled = function(static_cast<T&>(event)); 
			return true;
		}
		return false;
	}

};
