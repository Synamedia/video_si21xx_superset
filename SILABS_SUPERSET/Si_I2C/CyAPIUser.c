/***********************************************************************
Copyright 2021 Synamedia

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
 **********************************************************************/

#include <libusb-1.0/libusb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Silabs_L0_API.h"

#define VENDOR_IDENTIFIER  0x10c4
#define PRODUCT_IDENTIFIER 0x8497

#ifdef    USB_Capability
static pthread_mutex_t device_mutex;
static libusb_device_handle *device = NULL;
#endif /* USB_Capability */

int    Cypress_USB_Open     (void)
{
  int status = 0;
#ifdef    USB_Capability
  pthread_mutex_lock (&device_mutex);
  if (device == NULL)
    {
      status = libusb_init (NULL);
      if (status < 0)
        {
          SiTRACE ("%s: Failed to initialize libusb: %s.\n", __func__, libusb_strerror (status));
          status = 1;
          goto end;
        }
      device = libusb_open_device_with_vid_pid (NULL, VENDOR_IDENTIFIER, PRODUCT_IDENTIFIER);
      if (device == NULL)
        {
          SiTRACE ("%s: Failed to open USB device.\n", __func__);
          status = 1;
          goto end;
        }
    }
end:
  pthread_mutex_unlock (&device_mutex);
#endif /* USB_Capability */
  return status;
}

int    Cypress_USB_Close    (void)
{
#ifdef    USB_Capability
  pthread_mutex_lock (&device_mutex);
  if (device != NULL)
    {
      libusb_close (device);
      libusb_exit (NULL);
      device = NULL;
    }
  pthread_mutex_unlock (&device_mutex);
#endif /* USB_Capability */
  return 1;
}

int    Cypress_USB_LoadFPGA         (const char * sFilename )
{
  return -1;
}

int    Cypress_USB_LoadSPIoverPortA (unsigned char *SPI_data, unsigned char  length, unsigned short index)
{
  return -1;
}

int    Cypress_USB_LoadSPIoverGPIF  (unsigned char *SPI_data, int  length)
{
  return -1;
}

int    Cypress_USB_LoadSPIwaitDONE  (int  timeout_ms)
{
  return -1;
}

int    Cypress_USB_VendorCmd        (unsigned char  ucVendorCode, unsigned short value, unsigned short index, unsigned char * byteBuffer, int  len)
{
  return -1;
}

int    Cypress_USB_VendorRead       (unsigned char  ucVendorCode, unsigned short value, unsigned short index, unsigned char * byteBuffer, int *len)
{
  return -1;
}

#ifdef    USB_Capability
static int control_transfer (unsigned char   code,
                             unsigned char   endpoint,
                             unsigned char * data,
                             unsigned short  data_size)
{
  int status = 0;
  libusb_detach_kernel_driver (device, 0);
  status = libusb_claim_interface (device, 0);
  if (status != LIBUSB_SUCCESS)
    {
      SiTRACE ("%s: Failed to claim interface: %s.\n", __func__, libusb_strerror (status));
      status = 1;
      goto end;
    }
  status = libusb_control_transfer (device,
                                    endpoint
                                    | LIBUSB_REQUEST_TYPE_VENDOR
                                    | LIBUSB_RECIPIENT_DEVICE,
                                    code, 0x0, 0x0,
                                    data, data_size, 1000);
  if (status != (signed) data_size)
    {
      if (status < 0)
        {
          SiTRACE ("%s: Failed to send transfer: %s.\n", __func__, libusb_strerror (status));
          status = 1;
          goto release;
        }
      else
        {
          SiTRACE ("%s: Short transfer, should be %d, got %d.\n", __func__, data_size, status);
        }
    }
  status = 0;
release:
  libusb_release_interface (device, 0);
end:
  return 0;
}
#endif /* USB_Capability */

int    Cypress_USB_WriteI2C (
            unsigned char   ucPhysAddress,
            unsigned char   ucInternalAddressSize,
            unsigned char * pucInternalAddress,
            unsigned char   ucDataBufferSize,
            unsigned char * pucDataBuffer )
{
  int status = 0;
#ifdef    USB_Capability
  int index = 0;
  char data_size = 0;
  unsigned char * data = NULL;
  pthread_mutex_lock (&device_mutex);
  if (device == NULL)
    {
      SiTRACE ("%s: Failed to access USB device.\n", __func__);
      status = 1;
      goto end;
    }
  data_size = 2 + ucInternalAddressSize + ucDataBufferSize;
  data = malloc (data_size);
  if (data == NULL)
    {
      SiTRACE ("%s: Failed to allocate memory.\n", __func__);
      status = 1;
      goto end;
    }
  data[0] = ucPhysAddress;
  data[1] = ucInternalAddressSize + ucDataBufferSize;
  for (index = 0; index < ucInternalAddressSize; index++)
    {
      data[2 + index] = pucInternalAddress[index];
    }
  for (index = 0; index < ucDataBufferSize; index++)
    {
      data[2 + ucInternalAddressSize + index] = pucDataBuffer[index];
    }
  status = control_transfer (0xdb, LIBUSB_ENDPOINT_OUT, data, 2 + ucInternalAddressSize + ucDataBufferSize);
  free (data);
end:
  pthread_mutex_unlock (&device_mutex);
#endif /* USB_Capability */
  return status;
}

int    Cypress_USB_ReadI2C (
            unsigned char   ucPhysAddress,
            unsigned char   ucInternalAddressSize,
            unsigned char * pucInternalAddress,
            unsigned short  usDataBufferSize,
            unsigned char * pucDataBuffer )
{
  int status = 0;
#ifdef    USB_Capability
  int index = 0;
  char data_size = 0;
  unsigned char * data = NULL;
  pthread_mutex_lock (&device_mutex);
  if (device == NULL)
    {
      SiTRACE ("%s: Failed to access USB device.\n", __func__);
      status = 1;
      goto end;
    }
  data_size = 4 + ucInternalAddressSize;
  data = malloc (data_size);
  if (data == NULL)
    {
      SiTRACE ("%s: Failed to allocate memory.\n", __func__);
      status = 1;
      goto end;
    }
  memset (data, 0, data_size);
  data[0] = ucPhysAddress;
  data[1] = usDataBufferSize >> 8;
  data[2] = usDataBufferSize & 0xff;
  data[3] = ucInternalAddressSize;
  for (index = 0; index < ucInternalAddressSize; index++)
    {
      data[4 + index] = pucInternalAddress[index];
    }
  if (control_transfer (0xdc, LIBUSB_ENDPOINT_OUT, data, data_size))
    {
      SiTRACE ("%s: Read setup failed.\n", __func__);
      status = 1;
      goto free;
    }
  else
    {
      if (control_transfer (0xdd, LIBUSB_ENDPOINT_IN, pucDataBuffer, usDataBufferSize))
        {
          SiTRACE ("%s: Read operation failed.\n", __func__);
          status = 1;
          goto free;
        }
    }
free:
  free (data);
end:
  pthread_mutex_unlock (&device_mutex);
#endif /* USB_Capability */
  return status;
}

int    Cypress_USB_Command(char *cmd, char *text, double dval, double *retdval, char **rettxt)
{
  if (rettxt != NULL)
    {
      strncpy (*rettxt, "<unimplemented>", sizeof ("<unimplemented>"));
    }
  return 1;
}

int    Cypress_Configure  (char *cmd, char *text, double dval, double *retdval, char **rettxt)
{
  return -1;
}

int    Cypress_Cget       (char *cmd, char *text, double dval, double *retdval, char **rettxt)
{
  return -1;
}

char*  Cypress_Help       (void)
{
  return NULL;
}
