simple Tic8
{
    parameters:
        @display("i=block/routing");
    gates:
        input in;
        output out;
}

simple Toc8
{
    parameters:
        @display("i=block/process");
    gates:
        input in;
        output out;
}

network Tictoc8
{
    submodules:
        tic: Tic8 {
            parameters:
                @display("i=,cyan;p=72.9,76.950005");
        }
        toc: Toc8 {
            parameters:
                @display("i=,gold;p=189.54001,76.950005");
        }
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        tic.in <-- {  delay = 100ms; } <-- toc.out;
}
