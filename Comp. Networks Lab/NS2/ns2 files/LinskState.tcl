set ns [new Simulator]
set nf [open out.nam w]
set nr [open out.tr w]
$ns namtrace-all $nf
$ns trace-all $nr
proc finish {} {
	global ns nr nf
	$ns flush-trace
	close $nf
	close $nr
	#exec nam out.nam &
	exit 0
}

$ns color 1 Blue
$ns color 2 Red

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]

$ns duplex-link $n0 $n2 1Mb 10ms DropTail
$ns duplex-link $n1 $n3 1Mb 10ms DropTail
$ns duplex-link $n2 $n4 1Mb 10ms DropTail
$ns duplex-link $n3 $n4 1Mb 10ms DropTail
$ns duplex-link $n2 $n3 1Mb 10ms DropTail

$ns duplex-link-op $n0 $n2 orient center
$ns duplex-link-op $n1 $n3 orient center
$ns duplex-link-op $n2 $n4 orient center
$ns duplex-link-op $n3 $n4 orient center
$ns duplex-link-op $n2 $n3 orient down

set udp1 [new Agent/UDP]
$udp1 set fid_ 1
$udp1 set window_ 8000
$udp1 set packetsize_ 500
$ns attach-agent $n0 $udp1
set sink1 [new Agent/Null]
$ns attach-agent $n4 $sink1
$ns connect $udp1 $sink1

set cbr1 [new Application/Traffic/CBR]
$cbr1 set type_ CBR
$cbr1 set interval_ 0.005
$cbr1 set packetsize_ 500
$cbr1 attach-agent $udp1
 
set udp2 [new Agent/UDP]
$udp2 set fid_ 2 
$udp2 set window_ 8000
$udp2 set packetsize_ 500
$ns attach-agent $n1 $udp2
set sink2 [new Agent/Null]
$ns attach-agent $n4 $sink2
$ns connect $udp2 $sink2

set cbr2 [new Application/Traffic/CBR]
$cbr2 set interval_ 0.005
$cbr2 set packetsize_ 500
$cbr2 set type_ CBR
$cbr2 attach-agent $udp2

$ns rtproto LS

$ns rtmodel-at 3.0 down $n2 $n4
$ns rtmodel-at 5.0 up $n2 $n4
$ns rtmodel-at 6.0 down $n3 $n4
$ns rtmodel-at 8.0 up $n3 $n4

$ns at 0.1 "$cbr1 start"
$ns at 0.15 "$cbr2 start"
$ns at 9.0 "$cbr1 stop"
$ns at 9.0 "$cbr2 stop"
$ns at 10.0 "finish"
$ns run