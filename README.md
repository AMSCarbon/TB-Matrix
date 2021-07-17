# TB-Matrix
Work done as part of BABS3301 special research project. 

This project involved creating a computational model to simulate the spread of TB antibacterial resistance.

This is an agent based model, where the whole population is divided into sub populations and assigned to a vertex of a graph. 

The graph represents geographical distribution of a population, where neighbouring groups are more likely to interact. 

The main update loop is a big matrix multiplication, showing the likelihood of interactions between neighbouring verticies. 

Each update, an individual has a chance of becoming infected, becoming healthy, or dying. The disease also has a chance of gaining resistance.
