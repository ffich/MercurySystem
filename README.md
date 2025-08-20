## 📖 Description

**Mercury System** is a modular **⚙️ hardware + 💻 software development platform** tailored to streamline the creation of **embedded, connectivity, and IoT applications 🌐**.  
It uses a **LEGO-like 🧩 architecture**, where interchangeable boards can be combined to build powerful systems without reinventing the wheel.

---

### 🛠️ Hardware Architecture

The hardware is organized into distinct families:

- 🧠 **Base Board (BB)** – the “brain” of the system. It hosts the main microcontroller (e.g., PIC18F46J50), handles communication buses (I²C, UART), includes a LiPo battery charging circuit, USB bootloader, real-time clock, EEPROM, LEDs, buttons, and up to 11 GPIOs.

- 📡 **Modem Board (MB)** – connects the system to networks or devices. Variants include Wi-Fi, GSM/GPRS, Bluetooth, ZigBee, LoRa, and more. These interface with the BB via a dedicated serial/UART link.

- 🎛️ **Slave Board (SB)** – modular sensors or actuators, each with local microcontrollers that accept high-level command sets. They connect to the BB via I²C (multidrop) or UART and enable flexible expansions without pin conflicts.

- 🔋 **Power Board (PB)** – addresses power needs for more demanding setups. Variants support higher current requirements or alternative energy sources like solar or piezo harvesting.

- 🖥️ **Expansion Board (EB)** – supports planar connections of multiple boards and may include peripherals like displays or prototyping areas, offering an alternative to vertical stacking.

- 💡 **Brain-Less Board (BL)** – lightweight, cost-effective, controller-free boards for simple sensor or actuator functions that connect directly to BB GPIO

---

### Example of connection:

<img width="750" height="514" alt="image" src="https://github.com/user-attachments/assets/0d0fbb9c-dab1-4783-b8df-96d61bb99854" />

### Mercury Ecosystem:

<img width="750" height="472" alt="image" src="https://github.com/user-attachments/assets/2c5be269-0934-4889-a8ab-9d1dd64e1e17" />


---

### 🧑‍💻 Software Framework (MSF)

The **Mercury System Framework (MSF)** is a **layered software architecture** that abstracts hardware details and provides a clean structure for building embedded and IoT applications.  
It is designed to reduce boilerplate, increase portability, and allow developers to focus on **application logic** instead of low-level device drivers.

---

#### 🔌 HAL (Hardware Abstraction Layer)

- Provides drivers for low-level hardware blocks (GPIOs, timers, ADC, UART, I²C, SPI, …).  
- Encapsulates device-specific code so that higher layers remain portable.  
- Shields developers from MCU hardware differences.  
- Example: toggling an LED through HAL instead of directly touching registers.  

---

#### 🧩 SML (System Management Layer)

This layer manages all the **shared system resources** and services:

- **Bus Management** – controls communication interfaces (I²C, UART, SPI).  
- **Peripheral Management (PML)** 📶 – provides high-level APIs for **Modem Boards** and **Slave Boards**, so you can send commands without worrying about bus protocols.  
- **System Services (SSL)** ⚡ – manages power rails, LEDs, buttons, the RTC/RTCC, EEPROM, and the USB terminal.  
- **Communication Stacks** – handles Wi-Fi, Bluetooth, GSM/GPRS, LoRa, or ZigBee modems transparently.  

Thanks to SML, the developer can use modem/sensor boards interchangeably without rewriting core code.  

---

#### ⏱️ OSL (Operative System Layer)

Implements a **lightweight cooperative RTOS** tailored for Mercury projects:

- **Scheduler** – table-driven, periodic execution model (each task has a defined cycle time).  
- **Task Management** – applications are split into tasks with defined states (`InitializationState` and `RunningState`).  
- **Timers & Alarms** – software timers and alarms for periodic or one-shot operations.  
- **Event Handling** – inter-task communication via events and signals.  
- **Low Power Support** – tasks can be scheduled around power states to extend battery life.  

This gives the developer an **Arduino-like simplicity (setup/loop)** but with more structure, scalability, and multitasking capabilities.  

---

#### 📦 Application Layer

At the top sits the **user application**, which ties everything together:

- Uses HAL and SML APIs to control hardware and modems.  
- Registers tasks in the OSL scheduler to run periodically.  
- Implements business logic, e.g., *“read temperature sensor every 10s and send via GSM”*.  
- Designed to be easily portable across projects by reusing configuration and task code.  

---

### MSF Architecture:

<img width="750" height="623" alt="image" src="https://github.com/user-attachments/assets/d990db37-cc7c-4e4a-a282-3ea957c8e167" />


---

### 🚀 Advantages of MSF

- 🧩 **Separation of Concerns** – hardware, system services, OS, and application logic are clearly layered.  
- 🔄 **Reusability** – code can be reused across different projects and hardware setups.  
- 📚 **Consistency** – same API and coding model, regardless of the hardware used.  
- ⏳ **Rapid Prototyping** – focus only on high-level logic, while MSF handles drivers, services, and task scheduling.  
- 🌍 **IoT-Ready** – built-in support for modem boards means you can connect your project to the internet with minimal code changes.  


---

### 🛠️ Development Workflow

- 📥 **Setup** – includes documentation, framework libraries, demos, and tools.  
- 🏗️ **Project Generation** – automated scripts create MPLAB X projects linked to the framework.  
- ⚙️ **Configuration** – features enabled via `sys_cfg.h` (e.g., modem selection, task periods).  
- 🔁 **Coding Model** – dual-state (`InitializationState` / `RunningState`), similar to Arduino’s `setup()`/`loop()`.  
- 🚀 **Deployment** – choose between **Standalone** (direct programming) or **Bootloader** (USB `.hex` flashing).  

---

### 🌟 Benefits & Use Cases

- 🧩 **High Modularity** – easily extend hardware with new boards.  
- 📈 **Scalability** – bus-based expansion for sensors/actuators.  
- 🔋 **Efficient Power Management** – built-in LiPo support + external PBs.  
- ⏳ **Faster Development** – framework abstracts complexity so you focus on logic.  
- 🏠 🤖 🏭 **Applications** – home automation, robotics, industrial control, IoT devices, test benches, and more.  

---

### ✅ Summary

Mercury System combines **scalable modular hardware 🧩** with a **layered software framework 🏗️**, enabling developers to move quickly from idea to implementation.  
It’s an **IoT-ready platform 🌍** that balances flexibility, simplicity, and power—perfect for makers, engineers, and innovators.

