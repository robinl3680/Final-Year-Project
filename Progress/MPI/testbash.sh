#bash
echo "Starting with centralized..."
mpic++ Centralized.cpp -o Centralized
mpirun -np 101 --hostfile  Hostfile Centralized fourth.graph.part.100 fourth.graph
echo "Please note next going algorithms starting with core 0"
echo "Working on Peer to Peer..."
mpic++ PeerToPeer.cpp -o Peer
mpirun -np 100 --hostfile  Hostfile Peer fourth.graph.part.100 fourth.graph
echo "Greedy going to run....."
mpic++ Greedy.cpp -o Greedy
mpirun -np 100 --hostfile  Hostfile Greedy fourth.graph.part.100 fourth.graph
