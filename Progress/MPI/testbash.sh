#bash
echo "Starting with centralized..."
mpic++ Centralized.cpp -o Centralized
mpirun -np 101 --hostfile  Hostfile Centralized Partition/fourth.graph.part.100 Graph/fourth.graph
echo "\n"
echo "Please note next going algorithms starting with core 0"
echo "Working on Peer to Peer... \n"

mpic++ PeerToPeer.cpp -o Peer
mpirun -np 100 --hostfile Hostfile Peer Partition/fourth.graph.part.100 Graph/fourth.graph
echo "\n Greedy going to run \n"
mpic++ Greedy.cpp -o Greedy
mpirun -np 100 --hostfile Hostfile Greedy Partition/fourth.graph.part.100 Graph/fourth.graph
