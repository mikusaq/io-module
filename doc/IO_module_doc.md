# IO Module documentation

This is documentation for IO module and this module's devices.

## Module proposal

### Device

Devices supported by module includes these components:
 - inputs
 - outputs
 - button

Every device can have any number of these components, except for button, it is only one.

### Messages

#### Status

Status messages contains these informations about components:
 - array of inputs (the number of inputs on device determines array size)
    - number of input
    - state of input (true when input is high, else false)
 - array of outputs (the number of outputs on device determines array size)
    - number of output
    - bool value, which determines if error occurred on output
    - state of output (true when output is high, else false)
 - button
    - number of presses

Status is generated on Device and sent to Internal server (using Internal client api). The Fleet protocol ensures that the external app will receive the status.

> Status implementation is generic. That means that number of inputs and outputs is modifiable. This is achieved by using C++ template (the DeviceStatus class is template).

##### Class diagram

![](images/DeviceStatus_diagram.svg)

#### Command

Command messages contains:
 - list of outputAction messages, every outputAction message contains:
   - number of output, which will this action affect
   - ActionType enumeration, which could be SET_ON, SET_OFF, TOGGLE

Command is generated based on Status message and sent from external app to External server. The Fleet protocol ensures that the Device will receive the command. Device should execute action specified with ActionType on output with given number.

##### Class diagram

![](images/DeviceCommand_diagram.svg)

#### JSON messages

Statuses and commands decribed above will be transported as JSON strings. The key's strings are shortened to save some space on devices with limited memory.

##### Example of status JSON

```json
[
    [
        { "inNum": 1, "inSt": false },
        { "inNum": 2, "inSt": false },
        { "inNum": 3, "inSt": true },
        { "inNum": 4, "inSt": false },
        { "inNum": 5, "inSt": true },
        { "inNum": 6, "inSt": false }
    ]
    [
        { "outNum": 1, "outErr": false, "outSt": false },
        { "outNum": 2, "outErr": false, "outSt": true },
        { "outNum": 3, "outErr": false, "outSt": true },
        { "outNum": 4, "outErr": false, "outSt": false },
    ],
    { "butPr": 2 },
]
```

> The count of inputs and outputs is based on available components on the Device.

##### Example of command JSON

```json
[
    {"outNum": 1, "actType": 1},
    {"outNum": 2, "actType": 1},
    {"outNum": 3, "actType": 2}
]
```

## Devices

 - All devices uses Pull-up resistors on inputs. This means that when nothing is connected to input, it is always HIGH.
 - The size of incoming command is restricted on all devices (The size is different based on device's available memory)

### Arduino Opta

#### Device specs
 - input count : 6
 - output count: 4
 - one button
 - 4 LEDs - on device they are set on to indicate that output is set
 - max command size: 1024 bytes

#### Status forwarding

The Fleet protocol allows definition of conditions, when the Status will be forwarded to external app. For more information, read the Fleet protocol documentation.

Status will be forwarded to the external app when:
 - there is some difference between previous and current status (e.g. output has turned on, state of any input has changed)
 - buttonPresses in Status is higher than 0 or buttonPresses is 0 and buttonPresses in previous status was higher than 0.

### Arduino Mega R3

#### Device specs
 - input count : 4
 - output count: 4
 - no button (buttonPresses always 0)
 - max command size: 200 bytes

 > On Arduino Mega there are 16 available both inputs and outputs, but there is a problem with memory on Arduino Mega when using JSON statuses with all these values. So only 4 inputs and 4 outputs are used at this moment.
 To use all of the Mega's component, the memory problem must be resolved (with for example shortening the json - boolean->number, shorter key strings).

#### Status forwarding

Status will be forwarded to the external app when:
 - there is some difference between previous and current status (e.g. output has turned on, state of any input has changed)

### Arduino Uno R4 Minima

 > The implementation depends on using Arduino Uno R4 due to its bigger memory. Using R3 or lower revision may not work.

#### Device specs
 - input count : 6
 - output count: 7
 - no button (buttonPresses always 0)
 - max command size: 1024 bytes

 > Some pins are not used, because they are used by something else (pins: 4, 10, 11, 12, 13 used by Arduino ethernet shield, pins: 0, 1 used by UART).

#### Status forwarding

Status will be forwarded to the external app when:
 - there is some difference between previous and current status (e.g. output has turned on, state of any input has changed)
 