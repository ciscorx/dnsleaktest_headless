# dnsleaktest_headless

  Automates the process of going on a browser to www.dnsleaktest.com
    and finding out if your vpn is actually working.  Useful for
    headless servers that arent running a display manager.

  Tested on Raspberry Pi Buster with Chromium 83.0.4103.116
     The dnsleaktest_headless_vivaldi.sh script has been tested on Buster on 64 bit Vivaldi 2.0.1309.37
     
  Requirements: linux, xvfb, xautomation, scrot 0.8-18+,
    chromium-browser (optionally vivaldi-stable), Imagemagick (for
    optionally outputting screenshots), gcc (to optionally compile the
    c programs for marking up screen shots)

  It doesnt seem work properly when run as root.  Instead issue su user -c "./dnsleaktest_headless.sh" if you are root.

