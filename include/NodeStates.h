#pragma once
namespace NS {
	enum NodeState {
		PATH,
		WALL,
		GOAL,
		START,
		OPEN,
		CLOSED,
		CURRENT
	};
}