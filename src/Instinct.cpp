#include "Instinct.h"

Instinct::Instinct() {
	
}

Instinct::~Instinct() {
	m_Graph->~Graph();
}

void Instinct::reset() {
	m_Path.clear();	// Clear the old path, ready for a new one.
}

void Instinct::move() {

}

void Instinct::collided() {

}

void Instinct::markTarget(Position p_Position) {

}

void Instinct::markEnemy(Position p_Position) {

}

void Instinct::markBase(Position p_Position) {

}

void Instinct::markShell(Position p_Position) {

}

bool Instinct::isFiring() {
	return m_bFiring;
}

void Instinct::score(int p_ThisScore, int p_EnemyScore) {

}

Graph *Instinct::getGraph() const {
	return m_Graph;
}