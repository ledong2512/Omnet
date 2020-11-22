//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 6 of the License, or
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
//
// TODO documentation
//
simple Txc6
{
    parameters:
        bool sendMsgOnInit=default(false);
        int limit=default(2);
        @display("i=block/routing");
    gates:
        input in;
        output out;
}
network Tictoc6
{
    @display("bgb=261,156");
    submodules:
        tic: Txc6{
                sendMsgOnInit=true;
            @display("i=,cyan;p=68.67,65.96");
        }
        toc: Txc6{
            @display("i=,gold;p=169.56,65.96");
        }
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        tic.in <-- {  delay = 100ms; } <-- toc.out;

}

