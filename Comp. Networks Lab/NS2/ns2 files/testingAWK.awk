BEGIN {
	txsize=0
	recvdsize=0
	drpsize=0
	starttime=0
	stoptime=0
}
{
	event = $1
	time = $2
	level = $5

	

	if(level=="tcp" && (event == "+" || event == "s"))
	{
		if(time<starttime)
		{
			starttime=time
			printf("\ntime in starttime\n")
		}
		#printf("\ntime in starttime entered\n")
		txsize++
	}
	if(level=="tcp" && event=="r")
	{
		if(time>stoptime)
		{
			stoptime=time
		}
		recvdsize++
	}
	if(level=="tcp" && event=="d")
	{
		drpsize++
	}


}

END {
	printf("Avg.Throughput[kbps] = %.2f\ns = %.2f\nd = %.2f\nr = %.2f\nstartTime = %.2f\nstopTime = %.2f\n",
			recvdsize/(stoptime-starttime),txsize,drpsize,recvdsize,starttime,stoptime)
}