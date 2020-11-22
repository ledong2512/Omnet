simple WirelessNode
{
    parameters:
        int posX = default(0); 
        int posY =  default(0);
    @display("i=block/triangle,grey,2;is=vs");
    gates:
        input radioIn @directIn;
}
//
// TODO documentation
//
network WirelessA
{
    parameters:
        int n @prompt("Number of nodes") = default(100);
        
        @display("bgb=800,800;bgu=cm");
    submodules:
        node[n]: WirelessNode {
            parameters:
                
                @display("p=0,0");
        }
}

