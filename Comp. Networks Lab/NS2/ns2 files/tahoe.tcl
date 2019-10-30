#Create a simulator object
set ns [new Simulator]
set nr [open tahoe.tr w]
$ns trace-all $nr
#Open the nam trace file
set nf [open outt.nam w]
$ns namtrace-all $nf
$ns color 1 Blue
$ns color 2 Red
#Define a 'finish' procedure
proc finish {} {
global ns nf
$ns flush-trace
#Close the trace file
close $nf
#Execute nam on the trace file
#exec nam outt.nam &
exit 0
}
# Creating Nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
#Setting Links
$ns duplex-link $n0 $n1 10Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
#Setting Topology
$ns duplex-link-op $n0 $n1 orient left
$ns duplex-link-op $n1 $n2 orient left
#Setting Queue Limit
$ns queue-limit $n0 $n1 10
$ns queue-limit $n1 $n2 10
#Setup a TCP connection over 0 and 4 and its flow id, window size, packet size
set tcp [new Agent/TCP]
$tcp set fid_ 1
$tcp set window_ 8000
$tcp set packetSize_ 552
$ns attach-agent $n0 $tcp
set sink [new Agent/TCPSink/DelAck]
$ns attach-agent $n2 $sink
$ns connect $tcp $sink

#Setup a FTP over TCP connection
set ftp [new Application/FTP]
$ftp set type_ FTP
$ftp attach-agent $tcp

#Start and stop ftp
$ns at 0.1 "$ftp start"
$ns at 4.5 "$ftp stop"
#Call the finish procedure after 5 seconds of simulation time
$ns at 5.0 "finish"
#Run the simulation
$ns run