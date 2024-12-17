# doodle

**Developing graphical applications on embedded devices are annoying.** With all things UI/UX-based, iteration is key, and the requirement on hardware and reflashing the device for every iteration is a huge bottleneck -- it keeps you from making better applications.

`Doodle` is a framework that allows you to develop graphical applications on embedded devices without the need to reflash the device. By providing an abstraction layer between hardware and software, `Doodle` allows you to develop your application on your computer and run it on the device without reflashing. Simply change a few defines, and your application is portable between devices.