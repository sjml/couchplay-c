#pragma once

#include "actor.h"

void InitGL();
void TerminateGL();

void AddActor(Actor* a);

void GameLoop();
