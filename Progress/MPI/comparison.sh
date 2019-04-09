#bash
echo "Working on Peer to Peer... \n"

mpic++ PeerToPeer.cpp -o Peer
mpirun -np 100 --hostfile Hostfile Peer Partition/sixth.graph.part.100 Graph/sixth.graph
echo "\n Greedy going to run \n"
mpic++ Greedy.cpp -o Greedy
mpirun -np 100 --hostfile Hostfile Greedy Partition/sixth.graph.part.100 Graph/sixth.graph
