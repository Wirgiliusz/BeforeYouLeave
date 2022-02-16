# BeforeYouLeave
Reminder device – reminds about things that should be taken before leaving the home and sends remote notification in case of forgotten item.

---

## Project description
The goal of the project was to create a small electronic device which reminds the user about things that should be taken before leaving the home (for example: car keys, wallet, etc.) and will remotely notify him in case of forgotten item.
The device should be able to detect items, check if the user left the house, and send remote notification.

The most important parts of the projects are:
  - **ESP8266 microcontroller** with Wi-Fi connection.
  - **Mechanical hooks** for item detection.
  - **LEDs** giving information about current device status.
  - **Buttons** for overriding the status of the hooks.
  
### Device operation
The device can be hanged on the wall near home entrance. On the bottom there is series of hooks which allows to hang different items. On the front there is series of red LEDs - each for one hook. 
When the device detects item hanged on the hook it lights appropriate red LED to indicate that this item should be taken.
Finally the device is equipped with wireless communication that allows it to detect if user left the house and notify them in case they leave the home without one of the items (using phone notification).
Additionally each pair of hook and LED has button which allows to manually override the status – this is used to tell the device that user intentionally leaves one of the
items and does not want to be notified about it.
  
---

## Results

### Device
<img src=/docs/imgs/device_missing_item.jpg width="400"> <img src=/docs/imgs/device_on.jpg width="400"> 

### Electronic diagram
<img src=/docs/imgs/electronic_diagram.png width="700">



