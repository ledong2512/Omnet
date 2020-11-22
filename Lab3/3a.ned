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
simple Node
{
    parameters:
       // @display("i= block/routing");
    gates:
        input in[];
        output out[];

}
network MyName
{
    @display("bgb=1106,565");
    submodules:

        node: Node {
            @display("p=51,82");
        }
        node1: Node {
            @display("p=51,147");
        }
        node2: Node {
            @display("p=51,213");
        }
        node3: Node {
            @display("p=51,283");
        }
        node4: Node {
            @display("p=162,244");
        }
        node5: Node {
            @display("p=176,174");
        }
        node6: Node {
            @display("p=123,99");
        }
        node7: Node {
            @display("p=113,283");
        }
        node8: Node {
            @display("p=326,108");
        }
        node9: Node {
            @display("p=273,155");
        }
        node10: Node {
            @display("p=273,234");
        }
        node11: Node {
            @display("p=326,283");
        }
        node12: Node {
            @display("p=399,283");
        }
        node13: Node {
            @display("p=429,228");
        }
        node14: Node {
            @display("p=438,167");
        }
        node15: Node {
            @display("p=399,108");
        }
        node16: Node {
            @display("p=554,283");
        }
        node17: Node {
            @display("p=554,234");
        }
        node18: Node {
            @display("p=554,174");
        }
        node19: Node {
            @display("p=554,121");
        }
        node20: Node {
            @display("p=625,167");
        }
        node21: Node {
            @display("p=664,234");
        }
        node22: Node {
            @display("p=715,283");
        }
        node23: Node {
            @display("p=715,228");
        }
        node24: Node {
            @display("p=715,176");
        }
        node25: Node {
            @display("p=715,121");
        }
        node26: Node {
            @display("p=859,293");
        }
        node27: Node {
            @display("p=806,234");
        }
        node28: Node {
            @display("p=806,176");
        }
        node29: Node {
            @display("p=806,121");
        }
        node30: Node {
            @display("p=929,293");
        }
        node31: Node {
            @display("p=937,244");
        }
        node32: Node {
            @display("p=937,186");
        }
        node34: Node {
            @display("p=993,186");
        }
        node35: Node {
            @display("p=869,89");
        }
        node36: Node {
            @display("p=937,89");
        }
    connections:
        node5.out++ --> node6.in++;
        node.out++ --> node1.in++;
        node1.out++ --> node2.in++;
        node2.out++ --> node3.in++;
        node6.out++ --> node.in++;
        node3.out++ --> node7.in++;
        node7.out++ --> node4.in++;
        node4.out++ --> node9.in++;
        node9.out++ --> node8.in++;
        node8.out++ --> node15.in++;
        node15.out++ --> node14.in++;
        node14.out++ --> node13.in++;
        node13.out++ --> node12.in++;
        node12.out++ --> node11.in++;
        node11.out++ --> node10.in++;
        node10.out++ --> node9.in++;
        node14.out++ --> node16.in++;
        node16.out++ --> node17.in++;
        node17.out++ --> node18.in++;
        node18.out++ --> node19.in++;
        node19.out++ --> node20.in++;
        node20.out++ --> node21.in++;
        node21.out++ --> node22.in++;
        node22.out++ --> node23.in++;
        node23.out++ --> node24.in++;
        node24.out++ --> node25.in++;
        node25.out++ --> node29.in++;
        node29.out++ --> node35.in++;
        node35.out++ --> node36.in++;
        node36.out++ --> node35.in++;
        node35.out++ --> node29.in++;
        node29.out++ --> node28.in++;
        node28.out++ --> node27.in++;
        node27.out++ --> node26.in++;
        node26.out++ --> node30.in++;
        node30.out++ --> node31.in++;
        node31.out++ --> node32.in++;
        node32.out++ --> node34.in++;
}
