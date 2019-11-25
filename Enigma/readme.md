#bombe

##How I want it to work:
Engigma is implemented.

To implement the wiring of the enigma, we make the diagonal board, with
a class Wire representing each wire.
(the language is a little unspecific here, so bear(?) with me)
Each letter (A-Z) has a bundle of wires representing the value it is steckered to.
f. ex, there are 26 wires representing the value A is steckered to.
So, there are 26 bundles (A-Z) with 26 wires each (steckered to A-Z).
This is represented by an array of wires, Wire ***wires, where the first index is the bundle
and the second index is the wire in the bundle. We use a tripple pointer since we
want the array elements to be pointers to a specific wire.

The b-wire in the A bundle is connected to the a-wire in the B-bundle, the c-wire
in the A-bundle connected to the a-wire in the C-bundle ad so on. We achieve the
same result by letting wires[i][j]=wires[j][i] (note, pointing to the same object)

connecting wire w1 and w2 uses the syntax w1.connect(w2)
(or equivalently w2.connect(w1))

Using the map, we have to do some wiring. if A is connected to B at position
0, we have to connect all wires in the A bundle to the B bundle through a
cartridge(battery?) offset by 0. this is done through the function
connect_cartridge(Wire **bundle1, Wire **bundle2, Cartridge cart, int offset)
and has to be done for each step(XXX not good)

Ideally, we only need to do a single turn of one cartridge, and not turn all
cartridges for each step.
