Load Balancing is a challenging and interesting problem that appear in all High Performance Computing(HPC)environments. Most of the applications running in HPC environments use MPI based parallel computing. This approach needs load assignment, before starting the computations, which is difﬁcult in many problems where computational load changes dynamically.
In VSSC Aeronautics entity, Parallel computing is used to solve Computational Fluid Dynamics problems. Memory band width utilisation of CFD problems are very low, making them computationally inefﬁcient.
So, this prject is is concentrted on the following things,
Develop a distributed MPI frame work for solving computational problems efﬁciently. Challenge here is to design data layouts which can effectively utilise memory bandwidth. Multiple re-ordering schemes, along with data duplication can be attempted to improve the memory performance. While solving computational problems in a distributed MPI environment, it is important to ensure load balancing. But the static load balancing algorithms can fail if the computational loads vary overtime. A distributed work queue may be attempted to solve this problem


Get into the Progress folder all you need is there.
