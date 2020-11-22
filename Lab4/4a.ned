//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//
simple Switch
{
    parameters:
        int n = default(4);
        @display("i=abstract/router;is=s");
    gates:
        inout ports[n];
}
simple Host
{
    @display("i=device/wifilaptop;is=s");
    gates:
        inout port;
}
network FatTree
{
    parameters:
        int n = default(4);
        int n2 = int(n/2);
        int usPx = 2000-(n2*n2-1)*100;
        int asPx = 2000-(n2*n-1)*50;
        int hPx = 2000-(n2*n2*n-1)*25;
        @display("bgb=4000,1000");
    submodules:
        UpperSwitch[n2*n2]: Switch {
            @display("p=$usPx,30,r,200");
        }
        AggresiveSwitch[n2*n]: Switch {

            @display("p=$asPx,130,r,100");
        }
        EdgeSwitch[n2*n]: Switch {
            @display("p=$asPx,230,r,100");
        }
        Hosts[n2*n2*n]: Host {
            @display("p=$hPx,330,r,50");
        }
    connections:
        for k=0..((n)-1), for i=0..((n2)-1), for j=0..((n2)-1) {
            AggresiveSwitch[k*n2+i].ports[j] <--> EdgeSwitch[k*n2+j].ports[i];

        }
        for k=0..((n)-1), for i=0..((n2*n2)-1) {
            EdgeSwitch[k*n2+int(i/n2)].ports[n2+i%n2] <--> Hosts[k*n2*n2+i].port;
        }
        for k=0..((n)-1), for i=0..((n2*n2)-1) {
            UpperSwitch[i].ports[k] <--> AggresiveSwitch[k*n2+int(i/n2)].ports[n2+i%n2];
        }
}
