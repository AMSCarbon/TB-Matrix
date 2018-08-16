#pragma once

const float infectionRate = 0.000000001; //beta. 
const float progressionRate = 0.00002739726; //corresponds to 1 in 36500 chance of progressing. 10 years with 10% chance of progressing.
const float rapidProgressionProportion = 0.05; // percentage of individuals that transition without latent stage.
const float recoveryRate = 0.0056; // 1 in 180 chance of recovery. infections tend to last 3-8 months, average ~ 6 months. 
const float mutationRate = 0.000001; // transmission between states should probably be more specific. 
const float cost_of_resistance = 0.5; // how fit the mutants are relative to the normal strains.
const int numStrains = 5;
