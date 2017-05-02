klangsynthese
#############

This is the repository accompanying `Klangsynthese und -verarbeitung: Von der Theorie zur Programmierung <http://www.ak.tu-berlin.de/menue/lehre/wintersemester_201617/klangsynthese_und_verarbeitung_von_der_theorie_zur_programmierung/">`_.

PROJECT
_________
This project is a physical modeling synthesizer implementing a polyphonic Karplus-Strong Algorithm with fractional allpass delay.
Further features are: String bending, sustain mode, different excitation signals and burst window functions, a Wah-Wah filter and a FFTConvolution (guitar body IRs).

To start the application enter a file path to a wav-file (your impulse response) or type "noir" to bypass the FFTConvolution. For now the IR length should not be longer than 3 seconds.

MIDI control changes:

* `CC2: feedback gain value
* `CC3: sets the envelope shape (rect(default), tri, half-sine, hanning)
* `CC4: sets the envelope duration
* `CC5: sets oscillator mode (Sin, Saw, Square, Tri, Noise(default))
* `CC7: Wah-Wah cut-off
* `CC8: Wah-Wah Q
* `CC9: sustain on/off
* `CC10: release all notes
* `CC17: string bending (small variations of the main delay line length)

Also: Please use jack startup script "sh/jack_startup2.sh" for better performance.

Libraries
_________
The libraries needed for this to work properly are all installed on your `Raspberry Pi 3`_ (RPi3):

* `liblo (website)`_ as liblo-git_ (AUR_)
* `jackcpp (website)`_ as jackcpp-git_ (AUR_)
* `rtmidi (website)`_ as rtmidi_ (AUR_)
* `libsndfile (website)`_ as libsndfile_ (ALARM_)
* `yaml-cpp (website)`_ as yaml-cpp_ (ALARM_)
* `boost (website)`_ as boost_ (ALARM_)

Images
______
Images for the RPi3 can be found `here`_.
Please follow the `RPi3 installation instructions`_ for ALARM_ as a reference on how to install the latest image on a microSD card.

Hardware
________
We use specific hardware, that has been tested for low-latency audio with `JACK`_:

* RaspberryPi3
* `renkforce USB sound card`_

If you want to use different USB audio cards with `JACK`_, make sure to test them first and read up on how to start a `systemd user service`_ for `JACK`_.
There are plenty of examples in ~/.config/jack/.

Sources
_______
All sources can be found in the src/ directory.


Examples
________
All examples can be found in the examples/ directory.



.. _Raspberry Pi 3: https://www.raspberrypi.org/products/raspberry-pi-3-model-b/

.. _liblo (website): http://liblo.sourceforge.net/

.. _AUR: https://aur.archlinux.org/

.. _liblo-git: https://aur.archlinux.org/packages/liblo-git/

.. _jackcpp (website): http://x37v.info/jack_cpp/

.. _jackcpp-git: https://aur.archlinux.org/packages/jackcpp-git/

.. _rtmidi (website): http://www.music.mcgill.ca/~gary/rtmidi/

.. _rtmidi: https://aur.archlinux.org/packages/rtmidi/

.. _libsndfile (website): http://www.mega-nerd.com/libsndfile

.. _ALARM: https://archlinuxarm.org/

.. _libsndfile: https://archlinuxarm.org/packages/armv7h/libsndfile

.. _yaml-cpp (website): https://github.com/jbeder/yaml-cpp

.. _yaml-cpp: https://archlinuxarm.org/packages/armv7h/yaml-cpp

.. _boost (website): http://www.boost.org

.. _boost: https://archlinuxarm.org/packages/armv7h/boost

.. _here: https://www2.ak.tu-berlin.de/~drunge/klangsynthese

.. _RPi3 installation instructions: https://archlinuxarm.org/platforms/armv8/broadcom/raspberry-pi-3

.. _renkforce USB sound card: https://www.conrad.de/de/soundkarte-extern-renkforce-externe-kopfhoereranschluesse-1406215.html

.. _JACK: http://jackaudio.org/

.. _systemd user service: https://git.sleepmap.de/software/uenv.git/about/
