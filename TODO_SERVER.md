- [] Create a BLE Device

  - [] Import BLEDevice Library
  - [] Name Device
  - [] Initialize Device

- [] Create Server

  - [] Import BLEServer Library
  - [] Create BLEServer

- [] Create Server Callbacks

  - [] OnConnect
    - [] Turn on LED
  - [] OnDisconnect
    - [] Turn off LED

- [] Create Service

  - [] Define a Service UUID
  - [] Create a Service

- [] Create Characteristic

  - [] Define a Characteristic UUID
  - [] Create a Characteristic
  - [] Add Characteristic to Service

- [] Create a Characteristic Callback

  - [] Create subclass to override BLECharacteristicCallbacks
  - [] Send millis value every time request comes in

- [] Create a Descriptor

  - [] Add a Descriptor

- [] Advertise the Service

  - [] Get the Advertising object from Server
  - [] Set the properties of the Advertisement Data
  - [] Set the Advertisement Data to the Service
  - [] Add Service UUID
  - [] Start Advertising
