#bash
echo "Starting with centralized..."
mpic++ Centralized.cpp -o Centralized
mpirun -np 4 Centralized fourth.graph.part.3 fourth.graph
echo "Please note next going algorithms starting with core 0"
echo "Working on Peer to Peer..."
mpic++ PeerToPeer.cpp -o Peer
mpirun -np 3 Peer fourth.graph.part.3 fourth.graph
echo "Greedy going to run....."
mpic++ Greedy.cpp -o Greedy
mpirun -np 3 Greedy fourth.graph.part.3 fourth.graph
