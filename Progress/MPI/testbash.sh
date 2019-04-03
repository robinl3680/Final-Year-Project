#bash
echo "Starting with centralized..."
mpic++ Centralized.cpp -o Centralized
mpirun -np 4 Centralized graph1.graph.part.3 graph1.graph
echo "Please note next going algorithms starting with core 0"
echo "Working on Peer to Peer..."
mpic++ PeerToPeer.cpp -o Peer
mpirun -np 3 Peer graph1.graph.part.3 graph1.graph
echo "Greedy going to run....."
echo "Wait for a second...."
mpic++ Greedy.cpp -o Greedy
mpirun -np 3 Greedy graph1.graph.part.3 graph1.graph
