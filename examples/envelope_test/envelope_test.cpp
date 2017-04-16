#include <iostream>
#include <stdlib.h>
#include <cmath>

#include "../../src/envelope.h"

using std::cout;
using std::endl;
using namespace std;

class envelope_test{
private:

};

int main(){
Envelope *envelope;
envelope = new Envelope();

envelope -> setSampleRate(44100);
envelope -> setEnvDuration(0.001);

envelope->setEnvShape(TRI);


};
