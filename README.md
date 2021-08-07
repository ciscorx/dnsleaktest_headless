# dnsleaktest_headless

  Automates the process of going on a browser to www.dnsleaktest.com
    and finding out if your vpn is actually working.  Useful for
    headless servers that arent running a display manager.

  dnsleaktest_headless.sh script has been tested on:
    Raspberry Pi Buster with Chromium 83.0.4103.116,
    Odroid N2+ Alarm archlinux with Chromium 92.0.4515.131
    Odroid N2+ Ubuntu 20.10 on Xorg with Chromium 92.0.4515.107 snap
  
  dnsleaktest_headless_vivaldi.sh script has been tested on:
    Buster on 64 bit Vivaldi 2.0.1309.37
    Odroid N2+ on Ubuntu 20.10 on Xorg with Vivaldi 2.0.1309.37
    
  Requirements: linux, xvfb, xautomation, scrot 0.8-18+,
    chromium-browser (optionally vivaldi-stable)

  If the script doesnt work because it fails to recognize the button
    on the web page then simply remove the web page button recognition
    condition and replace it with true.  Chances are it will work

  It doesnt seem work properly when run as root.  Instead issue su user -c "./dnsleaktest_headless.sh" if you are root.

