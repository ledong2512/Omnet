simple Nodes
{
	parameters:
		@display("i=device/pc");
		int type;
		int des = default(-1);
		int k = 4;
		int EXB_SIZE=default(5);
		double TIMEOUT @unit(s)=default(1.0s);
		double TIME_INTERVAL @unit(s)=default(0.1ms);
		double CHANNEL_DELAY @unit(s)=default(0.0001s);
		double CREDIT_DELAY @unit(s)=default(1.0ns);
		double TIME_GEN_MSG @unit(s)=default(0.1ms);
		double TIME_OPERATION_OF_SWITCH @unit(s)=default(1000.0ns);
		//double TIME_UPDATE @unit(s)=default(100.0ns);
		double TIME_UPDATE @unit(s)=default(0.05s);
	gates:
		input in[4];
		output out[4];
}

network fattree
{
	parameters:
		Node[4].type = 2;
		Node[5].type = 2;
		Node[6].type = 2;
		Node[7].type = 2;
		Node[12].type = 2;
		Node[13].type = 2;
		Node[14].type = 2;
		Node[15].type = 2;
		Node[20].type = 2;
		Node[21].type = 2;
		Node[22].type = 2;
		Node[23].type = 2;
		Node[28].type = 2;
		Node[29].type = 2;
		Node[30].type = 2;
		Node[31].type = 2;
		Node[32].type = 2;
		Node[33].type = 2;
		Node[34].type = 2;
		Node[35].type = 2;
		Node[11].type = 1;
		Node[8].type = 3;
		Node[27].type = 1;
		Node[1].type = 3;
		Node[3].type = 1;
		Node[9].type = 3;
		Node[26].type = 1;
		Node[17].type = 3;
		Node[19].type = 1;
		Node[18].type = 3;
		Node[0].type = 1;
		Node[2].type = 3;
		Node[16].type = 1;
		Node[24].type = 3;
		Node[10].type = 1;
		Node[25].type = 3;

		Node[11].des = 8;
		Node[27].des = 1;
		Node[3].des = 9;
		Node[26].des = 24;
		Node[19].des = 18;
		Node[0].des = 2;
		Node[16].des = 17;
		Node[10].des = 25;
		Node[8].des = 11;
		Node[1].des = 27;
		Node[9].des = 3;
		Node[17].des = 16;
		Node[18].des = 19;
		Node[2].des = 0;
		Node[24].des = 26;
		Node[25].des = 10;
//		
//		
//		Node[0].des = 11;
//		Node[1].des = 19;
//		Node[2].des = 8;
//		Node[3].des = 24;
//		Node[8].des = 17;
//		Node[9].des = 25;
//		Node[10].des = 2;
//		Node[11].des = 18;
//		Node[16].des = 9;
//		Node[17].des = 27;
//		Node[18].des = 0;
//		Node[19].des = 26;
//		Node[24].des = 1;
//		Node[25].des = 3;
//		Node[26].des = 16;
//		Node[27].des = 10;

//		Node[0].des = 1;
//		Node[1].des = 2;
//		Node[2].des = 3;
//		Node[3].des = 8;
//		Node[8].des = 9;
//		Node[9].des = 10;
//		Node[10].des = 11;
//		Node[11].des = 16;
//		Node[16].des = 17;
//		Node[17].des = 18;
//		Node[18].des = 19;
//		Node[19].des = 24;
//		Node[24].des = 25;
//		Node[25].des = 26;
//		Node[26].des = 27;
//		Node[27].des = 0;
	types:
		 channel Channel extends ned.DatarateChannel {
			datarate = 1Gbps;
		}
	submodules:
		Node[36]: Nodes;
	connections allowunconnected:
		for p=0..3, for e=0..1, for s=0..1{
			Node[int(8*p + e*4/2 + s)].out++ --> Channel --> Node[int(8*p + 4*4/4 + e)].in++;
			Node[int(8*p + e*4/2 + s)].in++ <-- Channel <-- Node[int(8*p + 4*4/4 + e)].out++;
		}
		for p=0..3, for e=0..1, for s=2..3{
			Node[int(8*p + 4*4/4 + e)].out++ --> Channel --> Node[int(8*p + 4*4/4 + s)].in++;
			Node[int(8*p + 4*4/4 + e)].in++ <-- Channel <-- Node[int(8*p + 4*4/4 + s)].out++;
		}
//		for p=0..3, for s=2..3{
//			Node[int(8*p + 4*4/4 )].out++ --> Channel --> Node[int(8*p + 4*4/4 + s)].in++;
//			Node[int(8*p + 4*4/4 )].in++ <-- Channel <-- Node[int(8*p + 4*4/4 + s)].out++;
//		}
//		for p=0..3{
//			Node[int(8*p + 4*4/4 + 1)].out++ --> Channel --> Node[int(8*p + 4*4/4 + 3)].in++;
//			Node[int(8*p + 4*4/4 + 1)].in++ <-- Channel <-- Node[int(8*p + 4*4/4 + 3)].out++;
//			Node[int(8*p + 4*4/4 + 1)].out++ --> Channel --> Node[int(8*p + 4*4/4 + 2)].in++;
//			Node[int(8*p + 4*4/4 + 1)].in++ <-- Channel <-- Node[int(8*p + 4*4/4 + 2)].out++;
//		}
		
		
		for p=0..3, for e=0..1, for s=0..1{
			Node[int(8*p + 4*4/4 + 4/2 + e)].out++ --> Channel --> Node[int(e*4/2 + s + 32)].in++;
			Node[int(8*p + 4*4/4 + 4/2 + e)].in++ <-- Channel <-- Node[int(e*4/2 + s + 32)].out++;
		}
}
