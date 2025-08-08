import gc
import vfs
from flashbdev import bdev
from machine import USBDevice
try:
    if bdev:
        vfs.mount(bdev, "/")
except OSError:
    import inisetup

    inisetup.setup()

gc.collect()
usbd = USBDevice()
usbd.active(False)
