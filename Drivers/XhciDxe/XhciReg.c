/** @file

  The XHCI register operation routines.

Copyright (c) 2011 - 2017, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "Xhci.h"

/**
  Read 1-byte width XHCI capability register.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the 1-byte width capability register.

  @return The register content read.
  @retval If err, return 0xFF.

**/
UINT8
XhcReadCapReg8 (
  IN  USB_XHCI_INSTANCE   *Xhc,
  IN  UINT32              Offset
  )
{
  UINT8                   Data;
  EFI_STATUS              Status;

  Status = Xhc->PciIo->Mem.Read (
                             Xhc->PciIo,
                             EfiPciIoWidthUint8,
                             XHC_BAR_INDEX,
                             (UINT64) Offset,
                             1,
                             &Data
                             );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "XhcReadCapReg: Pci Io read error - %r at %d\n", Status, Offset));
    Data = 0xFF;
  }

  return Data;
}

/**
  Read 4-bytes width XHCI capability register.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the 4-bytes width capability register.

  @return The register content read.
  @retval If err, return 0xFFFFFFFF.

**/
UINT32
XhcReadCapReg (
  IN  USB_XHCI_INSTANCE   *Xhc,
  IN  UINT32              Offset
  )
{
  UINT32                  Data;
  EFI_STATUS              Status;

  Status = Xhc->PciIo->Mem.Read (
                             Xhc->PciIo,
                             EfiPciIoWidthUint32,
                             XHC_BAR_INDEX,
                             (UINT64) Offset,
                             1,
                             &Data
                             );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "XhcReadCapReg: Pci Io read error - %r at %d\n", Status, Offset));
    Data = 0xFFFFFFFF;
  }

  return Data;
}

/**
  Read 4-bytes width XHCI Operational register.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the 4-bytes width operational register.

  @return The register content read.
  @retval If err, return 0xFFFFFFFF.

**/
UINT32
XhcReadOpReg (
  IN  USB_XHCI_INSTANCE   *Xhc,
  IN  UINT32              Offset
  )
{
  UINT32                  Data;
  EFI_STATUS              Status;

//  ASSERT (Xhc->CapLength != 0);
  if (!Xhc->CapLength) {
    return 0xFFFFFFFF;
  }

  Status = Xhc->PciIo->Mem.Read (
                             Xhc->PciIo,
                             EfiPciIoWidthUint32,
                             XHC_BAR_INDEX,
                             (UINT64) (Xhc->CapLength + Offset),
                             1,
                             &Data
                             );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "XhcReadOpReg: Pci Io Read error - %r at %d\n", Status, Offset));
    Data = 0xFFFFFFFF;
  }

  return Data;
}

/**
  Write the data to the 4-bytes width XHCI operational register.

  @param  Xhc      The XHCI Instance.
  @param  Offset   The offset of the 4-bytes width operational register.
  @param  Data     The data to write.

**/
VOID
XhcWriteOpReg (
  IN USB_XHCI_INSTANCE    *Xhc,
  IN UINT32               Offset,
  IN UINT32               Data
  )
{
  EFI_STATUS              Status;

//  ASSERT (Xhc->CapLength != 0);
  if (!Xhc->CapLength) {
    return;
  }

  Status = Xhc->PciIo->Mem.Write (
                             Xhc->PciIo,
                             EfiPciIoWidthUint32,
                             XHC_BAR_INDEX,
                             (UINT64) (Xhc->CapLength + Offset),
                             1,
                             &Data
                             );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "XhcWriteOpReg: Pci Io Write error: %r at %d\n", Status, Offset));
  }
}

/**
  Write the data to the 2-bytes width XHCI operational register.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the 2-bytes width operational register.
  @param  Data         The data to write.

**/
VOID
XhcWriteOpReg16 (
  IN USB_XHCI_INSTANCE    *Xhc,
  IN UINT32               Offset,
  IN UINT16               Data
  )
{
  EFI_STATUS              Status;

//  ASSERT (Xhc->CapLength != 0);
  if (!Xhc->CapLength) {
    return;
  }

  Status = Xhc->PciIo->Mem.Write (
                             Xhc->PciIo,
                             EfiPciIoWidthUint16,
                             XHC_BAR_INDEX,
                             (UINT64) (Xhc->CapLength + Offset),
                             1,
                             &Data
                             );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "XhcWriteOpReg16: Pci Io Write error: %r at %d\n", Status, Offset));
  }
}

/**
  Read XHCI door bell register.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the door bell register.

  @return The register content read

**/
UINT32
XhcReadDoorBellReg (
  IN  USB_XHCI_INSTANCE   *Xhc,
  IN  UINT32              Offset
  )
{
  UINT32                  Data;
  EFI_STATUS              Status;

//  ASSERT (Xhc->DBOff != 0);
  if (!Xhc->DBOff) {
    return 0xFFFFFFFF;
  }

  Status = Xhc->PciIo->Mem.Read (
                             Xhc->PciIo,
                             EfiPciIoWidthUint32,
                             XHC_BAR_INDEX,
                             (UINT64) (Xhc->DBOff + Offset),
                             1,
                             &Data
                             );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "XhcReadDoorBellReg: Pci Io Read error - %r at %d\n", Status, Offset));
    Data = 0xFFFFFFFF;
  }

  return Data;
}

/**
  Write the data to the XHCI door bell register.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the door bell register.
  @param  Data         The data to write.

**/
VOID
XhcWriteDoorBellReg (
  IN USB_XHCI_INSTANCE    *Xhc,
  IN UINT32               Offset,
  IN UINT32               Data
  )
{
  EFI_STATUS              Status;

//  ASSERT (Xhc->DBOff != 0);
  if (!Xhc->DBOff) {
    return;
  }

  Status = Xhc->PciIo->Mem.Write (
                             Xhc->PciIo,
                             EfiPciIoWidthUint32,
                             XHC_BAR_INDEX,
                             (UINT64) (Xhc->DBOff + Offset),
                             1,
                             &Data
                             );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "XhcWriteOpReg: Pci Io Write error: %r at %d\n", Status, Offset));
  }
}

/**
  Read XHCI runtime register.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the runtime register.

  @return The register content read

**/
UINT32
XhcReadRuntimeReg (
  IN  USB_XHCI_INSTANCE   *Xhc,
  IN  UINT32              Offset
  )
{
  UINT32                  Data;
  EFI_STATUS              Status;

//  ASSERT (Xhc->RTSOff != 0);
  if (!Xhc->RTSOff) {
    return 0xFFFFFFFF;
  }

  Status = Xhc->PciIo->Mem.Read (
                             Xhc->PciIo,
                             EfiPciIoWidthUint32,
                             XHC_BAR_INDEX,
                             (UINT64) (Xhc->RTSOff + Offset),
                             1,
                             &Data
                             );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "XhcReadRuntimeReg: Pci Io Read error - %r at %d\n", Status, Offset));
    Data = 0xFFFFFFFF;
  }

  return Data;
}

/**
  Write the data to the XHCI runtime register.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the runtime register.
  @param  Data         The data to write.

**/
VOID
XhcWriteRuntimeReg (
  IN USB_XHCI_INSTANCE    *Xhc,
  IN UINT32               Offset,
  IN UINT32               Data
  )
{
  EFI_STATUS              Status;

//  ASSERT (Xhc->RTSOff != 0);
  if (!Xhc->RTSOff) {
    return;
  }

  Status = Xhc->PciIo->Mem.Write (
                             Xhc->PciIo,
                             EfiPciIoWidthUint32,
                             XHC_BAR_INDEX,
                             (UINT64) (Xhc->RTSOff + Offset),
                             1,
                             &Data
                             );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "XhcWriteRuntimeReg: Pci Io Write error: %r at %d\n", Status, Offset));
  }
}

/**
  Read XHCI extended capability register.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the extended capability register.

  @return The register content read

**/
UINT32
XhcReadExtCapReg (
  IN  USB_XHCI_INSTANCE   *Xhc,
  IN  UINT32              Offset
  )
{
  UINT32                  Data;
  EFI_STATUS              Status;

//  ASSERT (Xhc->ExtCapRegBase != 0);
  if (!Xhc->ExtCapRegBase) {
    return 0xFFFFFFFF;
  }

  Status = Xhc->PciIo->Mem.Read (
                             Xhc->PciIo,
                             EfiPciIoWidthUint32,
                             XHC_BAR_INDEX,
                             (UINT64) (Xhc->ExtCapRegBase + Offset),
                             1,
                             &Data
                             );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "XhcReadExtCapReg: Pci Io Read error - %r at %d\n", Status, Offset));
    Data = 0xFFFFFFFF;
  }

  return Data;
}

/**
  Write the data to the XHCI extended capability register.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the extended capability register.
  @param  Data         The data to write.

**/
VOID
XhcWriteExtCapReg (
  IN USB_XHCI_INSTANCE    *Xhc,
  IN UINT32               Offset,
  IN UINT32               Data
  )
{
  EFI_STATUS              Status;

//  ASSERT (Xhc->ExtCapRegBase != 0);
  if (!Xhc->ExtCapRegBase) {
    return;
  }

  Status = Xhc->PciIo->Mem.Write (
                             Xhc->PciIo,
                             EfiPciIoWidthUint32,
                             XHC_BAR_INDEX,
                             (UINT64) (Xhc->ExtCapRegBase + Offset),
                             1,
                             &Data
                             );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "XhcWriteExtCapReg: Pci Io Write error: %r at %d\n", Status, Offset));
  }
}


/**
  Set one bit of the runtime register while keeping other bits.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the runtime register.
  @param  Bit          The bit mask of the register to set.

**/
VOID
XhcSetRuntimeRegBit (
  IN USB_XHCI_INSTANCE    *Xhc,
  IN UINT32               Offset,
  IN UINT32               Bit
  )
{
  UINT32                  Data;

  Data  = XhcReadRuntimeReg (Xhc, Offset);
  Data |= Bit;
  XhcWriteRuntimeReg (Xhc, Offset, Data);
}

/**
  Clear one bit of the runtime register while keeping other bits.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the runtime register.
  @param  Bit          The bit mask of the register to set.

**/
VOID
XhcClearRuntimeRegBit (
  IN USB_XHCI_INSTANCE    *Xhc,
  IN UINT32               Offset,
  IN UINT32               Bit
  )
{
  UINT32                  Data;

  Data  = XhcReadRuntimeReg (Xhc, Offset);
  Data &= ~Bit;
  XhcWriteRuntimeReg (Xhc, Offset, Data);
}

/**
  Set one bit of the operational register while keeping other bits.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the operational register.
  @param  Bit          The bit mask of the register to set.

**/
VOID
XhcSetOpRegBit (
  IN USB_XHCI_INSTANCE    *Xhc,
  IN UINT32               Offset,
  IN UINT32               Bit
  )
{
  UINT32                  Data;

  Data  = XhcReadOpReg (Xhc, Offset);
  if (Data == 0xFFFFFFFF) {
    return;
  }
  Data |= Bit;
  XhcWriteOpReg (Xhc, Offset, Data);
}


/**
  Clear one bit of the operational register while keeping other bits.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the operational register.
  @param  Bit          The bit mask of the register to clear.

**/
VOID
XhcClearOpRegBit (
  IN USB_XHCI_INSTANCE    *Xhc,
  IN UINT32               Offset,
  IN UINT32               Bit
  )
{
  UINT32                  Data;

  Data  = XhcReadOpReg (Xhc, Offset);
  if (Data == 0xFFFFFFFF) {
    return;
  }
  Data &= ~Bit;
  XhcWriteOpReg (Xhc, Offset, Data);
}

/**
  Wait the operation register's bit as specified by Bit
  to become set (or clear).

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the operation register.
  @param  Bit          The bit of the register to wait for.
  @param  WaitToSet    Wait the bit to set or clear.
  @param  Timeout      The time to wait before abort (in millisecond, ms).

  @retval EFI_SUCCESS  The bit successfully changed by host controller.
  @retval EFI_TIMEOUT  The time out occurred.

**/
EFI_STATUS
XhcWaitOpRegBit (
  IN USB_XHCI_INSTANCE    *Xhc,
  IN UINT32               Offset,
  IN UINT32               Bit,
  IN BOOLEAN              WaitToSet,
  IN UINT32               Timeout
  )
{
  UINT32                  Index;
  UINT64                  Loop;

  Loop   = Timeout * XHC_1_MILLISECOND;

  for (Index = 0; Index < Loop; Index++) {
    if (XHC_REG_BIT_IS_SET (Xhc, Offset, Bit) == WaitToSet) {
      return EFI_SUCCESS;
    }

    gBS->Stall (XHC_1_MICROSECOND);
  }

  return EFI_TIMEOUT;
}

/**
  Set Bios Ownership

  @param  Xhc          The XHCI Instance.

**/
VOID
XhcSetBiosOwnership (
  IN USB_XHCI_INSTANCE    *Xhc
  )
{
  UINT32                    Buffer;
  INT32                     TimeOut;

  if (!Xhc || (Xhc->UsbLegSupOffset == 0xFFFFFFFF)) {
    return;
  }

//  DEBUG ((EFI_D_INFO, "XhcSetBiosOwnership: called to set BIOS ownership\n"));

// free from previous owner
  Buffer = XhcReadExtCapReg (Xhc, Xhc->UsbLegSupOffset);
  if (Buffer & USBLEGSP_BIOS_SEMAPHORE) {
    Buffer |= USBLEGSP_OS_SEMAPHORE;
    XhcWriteExtCapReg (Xhc, Xhc->UsbLegSupOffset, Buffer);

    TimeOut = 40;
    while (TimeOut--) {
      gBS->Stall(500);
      Buffer = XhcReadExtCapReg (Xhc, Xhc->UsbLegSupOffset);
      if ((Buffer & USBLEGSP_BIOS_SEMAPHORE) == 0) {
        break; // previous owner exit
      }
    }
  }
  Buffer = XhcReadExtCapReg (Xhc, Xhc->UsbLegSupOffset + 4);
  //
  // Disable all SMI in USBLEGCTLSTS
  //
  Buffer &= 0x1F1FEEU;
  Buffer |= 0xE0000000U;
  XhcWriteExtCapReg (Xhc, Xhc->UsbLegSupOffset + 4, Buffer);

// now clear all ownership
  Buffer = XhcReadExtCapReg (Xhc, Xhc->UsbLegSupOffset);
  Buffer &= ~(USBLEGSP_OS_SEMAPHORE | USBLEGSP_BIOS_SEMAPHORE);
  XhcWriteExtCapReg (Xhc, Xhc->UsbLegSupOffset, Buffer);
}

/**
  Clear Bios Ownership

  @param  Xhc       The XHCI Instance.

**/
VOID
XhcClearBiosOwnership (
  IN USB_XHCI_INSTANCE    *Xhc
  )
{
  UINT32                    Buffer;

  if (!Xhc || (Xhc->UsbLegSupOffset == 0xFFFFFFFF)) {
    return;
  }

//  DEBUG ((EFI_D_INFO, "XhcClearBiosOwnership: called to clear BIOS ownership\n"));

  Buffer = XhcReadExtCapReg (Xhc, Xhc->UsbLegSupOffset);
  Buffer = ((Buffer & (~USBLEGSP_BIOS_SEMAPHORE)) | USBLEGSP_OS_SEMAPHORE);
  XhcWriteExtCapReg (Xhc, Xhc->UsbLegSupOffset, Buffer);
}

/**
  Calculate the offset of the XHCI capability.

  @param  Xhc     The XHCI Instance.
  @param  CapId   The XHCI Capability ID.

  @return The offset of XHCI legacy support capability register.

**/
UINT32
XhcGetCapabilityAddr (
  IN USB_XHCI_INSTANCE    *Xhc,
  IN UINT8                CapId
  )
{
  UINT32 ExtCapOffset;
  UINT8  NextExtCapReg;
  UINT32 Data;

  ExtCapOffset = 0;

  do {
    //
    // Check if the extended capability register's capability id is USB Legacy Support.
    //
    Data = XhcReadExtCapReg (Xhc, ExtCapOffset);
    if ((Data & 0xFF) == CapId) {
      return ExtCapOffset;
    }
    //
    // If not, then traverse all of the ext capability registers till finding out it.
    //
    NextExtCapReg = (UINT8)((Data >> 8) & 0xFF);
    ExtCapOffset += (NextExtCapReg << 2);
  } while (NextExtCapReg != 0);

  return 0xFFFFFFFF;
}

/**
  Whether the XHCI host controller is halted.

  @param  Xhc     The XHCI Instance.

  @retval TRUE    The controller is halted.
  @retval FALSE   It isn't halted.

**/
BOOLEAN
XhcIsHalt (
  IN USB_XHCI_INSTANCE    *Xhc
  )
{
  return XHC_REG_BIT_IS_SET (Xhc, XHC_USBSTS_OFFSET, XHC_USBSTS_HALT);
}


/**
  Whether system error occurred.

  @param  Xhc      The XHCI Instance.

  @retval TRUE     System error happened.
  @retval FALSE    No system error.

**/
BOOLEAN
XhcIsSysError (
  IN USB_XHCI_INSTANCE    *Xhc
  )
{
  return XHC_REG_BIT_IS_SET (Xhc, XHC_USBSTS_OFFSET, XHC_USBSTS_HSE);
}

/**
  Reset the XHCI host controller.

  @param  Xhc          The XHCI Instance.
  @param  Timeout      Time to wait before abort (in millisecond, ms).

  @retval EFI_SUCCESS  The XHCI host controller is reset.
  @return Others       Failed to reset the XHCI before Timeout.

**/
EFI_STATUS
XhcResetHC (
  IN USB_XHCI_INSTANCE    *Xhc,
  IN UINT32               Timeout
  )
{
  EFI_STATUS              Status;

  Status = EFI_SUCCESS;

  DEBUG ((EFI_D_INFO, "XhcResetHC!\n"));
  //
  // Host can only be reset when it is halt. If not so, halt it
  //
  if (!XHC_REG_BIT_IS_SET (Xhc, XHC_USBSTS_OFFSET, XHC_USBSTS_HALT)) {
    Status = XhcHaltHC (Xhc, Timeout);

    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  if ((Xhc->DebugCapSupOffset == 0xFFFFFFFF) || ((XhcReadExtCapReg (Xhc, Xhc->DebugCapSupOffset) & 0xFF) != XHC_CAP_USB_DEBUG) ||
      ((XhcReadExtCapReg (Xhc, Xhc->DebugCapSupOffset + XHC_DC_DCCTRL) & BIT0) == 0)) {
    XhcSetOpRegBit (Xhc, XHC_USBCMD_OFFSET, XHC_USBCMD_RESET);
    //
    // Some XHCI host controllers require to have extra 1ms delay before accessing any MMIO register during reset.
    // Otherwise there may have the timeout case happened.
    // The below is a workaround to solve such problem.
    //
    gBS->Stall (XHC_1_MILLISECOND);
    Status = XhcWaitOpRegBit (Xhc, XHC_USBCMD_OFFSET, XHC_USBCMD_RESET, FALSE, Timeout);
  }

  return Status;
}


/**
  Halt the XHCI host controller.

  @param  Xhc          The XHCI Instance.
  @param  Timeout      Time to wait before abort (in millisecond, ms).

  @return EFI_SUCCESS  The XHCI host controller is halt.
  @return EFI_TIMEOUT  Failed to halt the XHCI before Timeout.

**/
EFI_STATUS
XhcHaltHC (
  IN USB_XHCI_INSTANCE   *Xhc,
  IN UINT32              Timeout
  )
{
  EFI_STATUS              Status;

  XhcClearOpRegBit (Xhc, XHC_USBCMD_OFFSET, XHC_USBCMD_RUN);
  Status = XhcWaitOpRegBit (Xhc, XHC_USBSTS_OFFSET, XHC_USBSTS_HALT, TRUE, Timeout);
  return Status;
}


/**
  Set the XHCI host controller to run.

  @param  Xhc          The XHCI Instance.
  @param  Timeout      Time to wait before abort (in millisecond, ms).

  @return EFI_SUCCESS  The XHCI host controller is running.
  @return EFI_TIMEOUT  Failed to set the XHCI to run before Timeout.

**/
EFI_STATUS
XhcRunHC (
  IN USB_XHCI_INSTANCE    *Xhc,
  IN UINT32               Timeout
  )
{
  EFI_STATUS              Status;

  XhcSetOpRegBit (Xhc, XHC_USBCMD_OFFSET, XHC_USBCMD_RUN);
  Status = XhcWaitOpRegBit (Xhc, XHC_USBSTS_OFFSET, XHC_USBSTS_HALT, FALSE, Timeout);
  return Status;
}

/**
 Perform Intel-specific Quirks

 @param  PciIo   The PciIo of Xhc Instance.
 **/
VOID
XhcIntelQuirks (
  IN EFI_PCI_IO_PROTOCOL  *PciIo
  )
{
	EFI_STATUS Status;
	UINT32 Regs[4];
	struct {
		UINT16 Vendor;
		UINT16 Device;
	} Ids;

	if (!PciIo)
		return;

	Status = PciIo->Pci.Read(PciIo, EfiPciIoWidthUint16, 0U, 2U, &Ids);
	if (EFI_ERROR(Status) || Ids.Vendor != 0x8086U)
		return;
	switch (Ids.Device) {
		case 0x1E31U:	// Panther Point
		case 0x8C31U:	// Lynx Point
		case 0x8CB1U:	// Wildcat Point
		case 0x8D31U:	// X99
			Status = PciIo->Pci.Read(PciIo, EfiPciIoWidthUint32, 0xD0U, 4U, &Regs[0]);
			if (EFI_ERROR(Status))
				break;
			Regs[1] |= Regs[0];
			Regs[3] |= Regs[2];
			if (Regs[1] != Regs[0])
				(VOID) PciIo->Pci.Write(PciIo, EfiPciIoWidthUint32, 0xD0U, 1U, &Regs[1]);
			if (Regs[3] != Regs[2])
				(VOID) PciIo->Pci.Write(PciIo, EfiPciIoWidthUint32, 0xD8U, 1U, &Regs[3]);
			break;
		default:
			break;
	}
}
