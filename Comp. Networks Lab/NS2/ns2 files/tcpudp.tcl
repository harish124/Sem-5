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
	exec nam out.nam &
	exit 0
} 
$ns color 1 Red
$ns color 2 Blue
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]

$ns duplex-link $n0 $n2 10Mb 10ms DropTail
$ns duplex-link $n1 $n2 10Mb 10ms DropTail
$ns duplex-link $n2 $n3 0.3Mb 100ms DropTail
$ns duplex-link $n3 $n4 10Mb 10ms DropTail
$ns duplex-link $n3 $n5 10Mb 10ms DropTail

$ns duplex-link-op $n2 $n3 orient center
$ns duplex-link-op $n0 $n2 orient right-up
$ns duplex-link-op $n1 $n2 orient right-down
$ns duplex-link-op $n3 $n4 orient right-up
$ns duplex-link-op $n3 $n5 orient right-down

$ns queue-limit $n2 $n3 10

set tcp [new Agent/TCP/Newreno]
$tcp set fid_ 1
$tcp set window_ 8000
$tcp set packetsize_ 552
$ns attach-agent $n0 $tcp
set tcpsink [new Agent/TCPSink/DelAck]
$ns attach-agent $n4 $tcpsink
$ns connect $tcp $tcpsink

set ftp [new Application/FTP]
$ftp set type_ FTP
#interval
#packetsize
$ftp attach-agent $tcp


set udp [new Agent/UDP]
$udp set fid_ 2
$ns attach-agent $n1 $udp
set udpsink [new Agent/Null]
$ns attach-agent $n5 $udpsink
$ns connect $udp $udpsink

set cbr [new Application/Traffic/CBR]
$cbr set type_ CBR
$cbr set packetsize_ 1000
$cbr set rate_ 0.01mb
#$cbr set interval_ 1ms
$cbr set random_ false
$cbr attach-agent $udp

$ns at 0.1 "$cbr start"
$ns at 0.2 "$ftp start"
$ns at 4.8 "$cbr stop"
$ns at 5.0 "$ftp stop"
$ns at 5.0 "finish"
$ns run