#include "Achtung.h"

Achtung::Achtung(QObject* parent) 
	: QObject(parent)
{
	connect(&logic_, &Game::Logic::applySettings, this, &Achtung::applySettings);
	connect(this, &Achtung::applySettings, &logic_, &Game::Logic::drawTickTransition);
	connect(this, &Achtung::keyPressed,    &logic_, &Game::Logic::localPlayerKeyPressed);
	connect(this, &Achtung::keyReleased,   &logic_, &Game::Logic::localPlayerKeyReleased);
}

void Achtung::setDrawingTarget(gsl::not_null<GameCanvas*> gameCanvas)
{
	gameCanvas->setRenderImage(logic_.getBoardImage());
	connect(this,       &Achtung::applySettings, gameCanvas, &GameCanvas::updateBoardSize);
	connect(gameCanvas, &GameCanvas::drawHeads,  &logic_,    &Game::Logic::drawPlayerHeads, Qt::DirectConnection);
}

void Achtung::start()
{
	logic_.startGame();
}

void Achtung::stop()
{
	//disconnect actually
	logic_.stopGame();
}