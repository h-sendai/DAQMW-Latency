// -*- C++ -*-
/*!
 * @file InPort.h
 * @brief InPort template class
 * @date $Date: 2007-12-31 03:08:03 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2003-2008
 *     Noriaki Ando
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id: InPort.h 1865 2010-02-15 01:23:02Z kurihara $
 *
 */

#ifndef RTC_INPORT_H
#define RTC_INPORT_H

#include <string>
#include <vector>
#include <iostream>

#include <coil/TimeValue.h>
#include <coil/Time.h>
#include <coil/OS.h>

#include <rtm/RTC.h>
#include <rtm/Typename.h>
#include <rtm/InPortBase.h>
#include <rtm/CdrBufferBase.h>
#include <rtm/PortCallback.h>
#include <rtm/InPortConnector.h>

namespace RTC
{
  /*!
   * @if jp
   *
   * @class InPort
   *
   * @brief InPort $B%F%s%W%l!<%H%/%i%9(B
   * 
   * InPort $B$N<BAu$G$"$k(B InPort<T> $B$N%F%s%W%l!<%H%/%i%9!#(B
   * <T> $B$O(BBasicDataType.idl $B$K$FDj5A$5$l$F$$$k7?$G!"%a%s%P$H$7$F(B
   * Time $B7?$N(B tm , $B$*$h$S(B T$B7?$N(B data $B$r;}$D9=B$BN$G$J$/$F$O$J$i$J$$!#(B
   * InPort $B$OFbIt$K%j%s%0%P%C%U%!$r;}$A!"30It$+$iAw?.$5$l$?%G!<%?$r=g<!(B
   * $B$3$N%j%s%0%P%C%U%!$K3JG<$9$k!#%j%s%0%P%C%U%!$N%5%$%:$O%G%U%)%k%H$G(B64$B$H(B
   * $B$J$C$F$$$k$,!"%3%s%9%H%i%/%?0z?t$K$h$j%5%$%:$r;XDj$9$k$3$H$,$G$-$k!#(B
   * $B%G!<%?$O%U%i%0$K$h$C$FL$FI!"4{FI>uBV$,4IM}$5$l!"(BisNew(), write(), read(),
   * isFull(), isEmpty() $BEy$N%a%=%C%I$K$h$j%O%s%I%j%s%0$9$k$3$H$,$G$-$k!#(B
   *
   *
   *   
   * OnRead$B7O%3!<%k%P%C%/(B ($BFI$_=P$7$K5/0x$9$k%$%Y%s%H$K$h$j%3!<%k$5$l$k(B)
   *
   * - void OnRead::operator(): 
   *     InPort::read() $B$r8F$S=P$7FI$_=P$7$r9T$&:]$K%3!<%k$5$l$k!#(B
   *
   * - DataType OnReadConvert::operator(DataType): 
   *     InPort::read() $B$r8F$S=P$7!"%G!<%?$r%P%C%U%!$+$iFI$_$@$9:]$K8F$P$l(B
   *     $B%G!<%?$NJQ49$r9T$&!#0z?t$K$O%P%C%U%!$+$iFI$_=P$5$l$?CM$,M?$($i$l!"(B
   *     $BJQ498e$N%G!<%?$rLa$jCM$H$7$FJV$9!#$3$NCM$,(Bread()$B$NJV$9CM$H$J$k!#(B
   *
   *
   * OnBuffer$B7O%3!<%k%P%C%/(B ($B%P%C%U%!$K5/0x$9$k%$%Y%s%H$K$h$j%3!<%k$5$l$k(B)
   *
   * - void OnBufferOverwrite::operator(ConnectorId):
   *     ConnectorBase::write() $B%3!<%k;~$K!"%P%C%U%!$,>e=q$-%b!<%I$K@_Dj(B
   *     $B$5$l$F$*$j!"%P%C%U%!$,$$$C$Q$$$N>l9g$K8F$S=P$5$l$k%3!<%k%P%C%/!#(B
   *     $B%3%M%/%?$N(BID$B$,0z?t$KM?$($i$l$k!#(B
   *
   * - void OnBufferEmpty::operator(ConnectorId): 
   *     $B%3%M%/%?$,%P%C%U%!$rFI$_$@$9:]$K!"%P%C%U%!$,6u$N>l9g$K8F$S=P$5$l$k!#(B
   *     $B%3%M%/%?$N(BID$B$,0z?t$KM?$($i$l$k!#(B
   *
   * - void OnBufferReadTimeout::operator(ConnectorId):
   *     $B%3%M%/%?$,%P%C%U%!$rFI$_$@$9:]$K!"%P%C%U%!$,6u$G$+$D!"FI$_=P$7$,(B
   *     $B%?%$%`%"%&%H$7$?>l9g$K8F$S=P$5$l$k%3!<%k%P%C%/!#%3%M%/%?$N(BID
   *     $B$,0z?t$KM?$($i$l$k!#(B
   *
   * - void OnBufferFull::operator(ConnectorId, DataType):
   *     ConnectorBase::write() $B%3!<%k;~$K!"%P%C%U%!$,$$$C$Q$$$N>l9g$K(B
   *     $B8F$S=P$5$l$k%3!<%k%P%C%/!#%3%M%/%?$N(BID$B$*$h$S=q$-9~$a$J$+$C$?%G!<%?(B
   *     $B$,0z?t$KM?$($i$l$k!#(B
   *
   * - void OnBufferWriteTimeout::operator(ConnectorId, DataType):
   *     ConnectorBase::write() $B%3!<%k;~$K!"%P%C%U%!$,$$$C$Q$$$G=q9~$_$,(B
   *     $B%?%$%`%"%&%H$7$?>l9g$K8F$S=P$5$l$k%3!<%k%P%C%/!#%3%M%/%?$N(BID
   *     $B$*$h$S=q$-9~$a$J$+$C$?%G!<%?$,0z?t$KM?$($i$l$k!#(B
   *
   *
   * OnConnect$B7O%3!<%k%P%C%/(B ($B@\B3$K5/0x$9$k%$%Y%s%H$K$h$j%3!<%k$5$l$k(B)
   * 
   * - void OnConnect::operator(ConnectorProfile):
   *      $B%]!<%H$N@\B3;~$K8F$S=P$5$l$k%3!<%k%P%C%/!#0z?t$K(BConnectorProfile
   *      $B$,M?$($i$l$k!#(B
   *
   * - void OnDisconnect::operator(ConnectorId):
   *      $B%]!<%H$N@\B3@ZCG;~$K8F$S=P$5$l$k%3!<%k%P%C%/!#0z?t$K%3%M%/%?(BID
   *      $B$,M?$($i$l$k!#(B  
   *
   * - void OnConnectionLost::operator(ConnectorId):
   *      $B%]!<%H$N@\B3$,%m%9%H$7$?>l9g$K8F$S=P$5$l$k%3!<%k%P%C%/!#(B
   *      $B0z?t$K%3%M%/%?(BID$B$,M?$($i$l$k!#(BInPort$B$O!"Aj<jB&(BOutPort$B$H$N(B
   *      $B@\B3$r%m%9%H$7$?>l9g!"@\B3$r6/@)E*$K@ZCG$9$k$N$G!"(B
   *      $B0z$-B3$-(B OnDisconnect $B%3!<%k%P%C%/$,8F$S=P$5$l$k!#(B
   *
   *
   * OnReceive$B7O%3!<%k%P%C%/(B ($BAw?.B&$K5/0x$9$k%$%Y%s%H$K$h$j%3!<%k$5$l$k(B)
   *
   * - void OnSend::operator(ConnectorId):
   *      $B%G!<%?$,(BInPort$B$KBP$7$FAw$i$l$k:]$K8F$S=P$5$l$k%3!<%k%P%C%/!#(B
   *      $B0z?t$K%3%M%/%?(BID$B$,M?$($i$l$k!#(B
   *
   * - void OnSenderTimeout::operator(ConnectorId):
   *      $B%G!<%?$,(BInPort$B$KBP$7$FAw$i$l$?$,!"Aw?.$,%?%$%`%"%&%H$7$?:]$K(B
   *      $B8F$S=P$5$l$k%3!<%k%P%C%/!#0z?t$K%3%M%/%?(BID$B$,M?$($i$l$k!#(B
   *      InPort$B$N%W%m%P%$%@$*$h$S%3%s%7%e!<%^$,BP1~$7$F$$$k>l9g$K8B$jM-8z!#(B
   *
   * - void OnSenderError::operator(ConnectorId):
   *      $B%G!<%?$,(BInPort$B$KBP$7$FAw$i$l$?$,!"2?$i$+$N%(%i!<$r(BInPort$BB&$,(B
   *      $BJV$7$?>l9g$K8F$S=P$5$l$k%3!<%k%P%C%/!#(B
   *      $B0z?t$K$O!"%3%M%/%?(BID$B$,M?$($i$l$k!#(B
   *
   *
   * OnReceive$B7O%3!<%k%P%C%/(B ($B<u?.B&$K5/0x$9$k%$%Y%s%H$K$h$j%3!<%k$5$l$k(B)
   * 
   * - void OnReceiverFull::operator(ConnectorId):
   *      $B%G!<%?$,(BInPort$B$KAw$i$l$k$b$N$N!"(BInPort$BB&$N%P%C%U%!$,$$$C$Q$$$N>l9g$K(B
   *      $B$3$l$rDLCN$9$k$?$a$K8F$P$l$k%3!<%k%P%C%/!#(B
   *       $B0z?t$K$O!"%3%M%/%?(BID$B$,M?$($i$l$k!#(B
   *
   * - void OnReceiverTimeout::operator(ConnectorId):
   *      $B%G!<%?$,(BInPort$B$KAw$i$l$k$b$N$N!"(BInPort$BB&$N%P%C%U%!$,$$$C$Q$$$G(B
   *      $B%?%$%`%"%&%H$7$?>l9g$K$3$l$rDLCN$9$k$?$a$K8F$P$l$k%3!<%k%P%C%/!#(B
   *       $B0z?t$K$O!"%3%M%/%?(BID$B$,M?$($i$l$k!#(B
   *
   * - void OnReceiverError::operator(ConnectorId):
   *       $B%G!<%?$,(BInPort$B$KAw$i$l$k$b$N$N!"(BInPort$BB&$G2?$i$+$N%(%i!<$rJV$7$?(B
   *       $B>l9g$K8F$S=P$5$l$k%3!<%k%P%C%/!#(B
   *       $B0z?t$K$O!"%3%M%/%?(BID$B$,M?$($i$l$k!#(B
   *
   * - void OnReceived::operator(ConnectorId):
   *       $B%G!<%?$NAw?.$*$h$S<u?.$,40N;$7$?:]$K8F$S=P$5$l$k%3!<%k%P%C%/!#(B
   *       $B0z?t$K$O!"%3%M%/%?(BID$B$,M?$($i$l$k!#(B
   *
   *
   * @since 0.2.0
   *
   * @else
   *
   * @class InPort
   *
   * @brief InPort template class
   *
   * This is a template class that implements InPort.
   * <T> is the type defined in BasicDataType.idl and must be the structure which
   * has both Time type tm and type-T data as a member. InPort has a ring
   * buffer internally, and stores the received data externally in this buffer
   * one by one. The size of ring buffer can be specified according to 
   * the argument of constructor, though the default size is 64. Unread
   * data and data which is already read are managed with the flag, and the
   * data can be handled by the isNew(), write(), read(), isFull() and isEmpty()
   * method etc. 
   *
   * @since 0.2.0
   *
   * @endif
   */
  template <class DataType>
  class InPort
    : public InPortBase
  {
  public:
    DATAPORTSTATUS_ENUM
    /*!
     * @if jp
     *
     * @brief $B%3%s%9%H%i%/%?(B
     *
     * $B%3%s%9%H%i%/%?!#(B
     * $B%Q%i%a!<%?$H$7$FM?$($i$l$k(B T $B7?$NJQ?t$K%P%$%s%I$5$l$k!#(B
     *
     * @param name InPort $BL>!#(BInPortBase:name() $B$K$h$j;2>H$5$l$k!#(B
     * @param value $B$3$N(B InPort $B$K%P%$%s%I$5$l$k(B T $B7?$NJQ?t(B
     * @param bufsize InPort $BFbIt$N%j%s%0%P%C%U%!$N%P%C%U%!D9(B($B%G%U%)%k%HCM(B:64)
     * @param read_block $BFI9~%V%m%C%/%U%i%0!#(B
     *        $B%G!<%?FI9~;~$KL$FI%G!<%?$,$J$$>l9g!"<!$N%G!<%?<u?.$^$G%V%m%C%/$9$k(B
     *        $B$+$I$&$+$r@_Dj(B($B%G%U%)%k%HCM(B:false)
     * @param write_block $B=q9~%V%m%C%/%U%i%0!#(B
     *        $B%G!<%?=q9~;~$K%P%C%U%!$,%U%k$G$"$C$?>l9g!"%P%C%U%!$K6u$-$,$G$-$k(B
     *        $B$^$G%V%m%C%/$9$k$+$I$&$+$r@_Dj(B($B%G%U%)%k%HCM(B:false)
     * @param read_timeout $BFI9~%V%m%C%/$r;XDj$7$F$$$J$$>l9g$N!"%G!<%?FI<h%?%$%`(B
     *        $B%"%&%H;~4V(B($B%_%jIC(B)($B%G%U%)%k%HCM(B:0)
     * @param write_timeout $B=q9~%V%m%C%/$r;XDj$7$F$$$J$$>l9g$N!"%G!<%?=q9~%?%$%`(B
     *        $B%"%&%H;~4V(B($B%_%jIC(B)($B%G%U%)%k%HCM(B:0)
     *
     * @else
     *
     * @brief A constructor.
     *
     * constructor.
     * This is bound to type-T variable given as a parameter.
     *
     * @param name A name of the InPort. This name is referred by
     *             InPortBase::name().
     * @param value type-T variable that is bound to this InPort.
     * @param bufsize Buffer length of internal ring buffer of InPort
     *                (The default value:64)
     * @param read_block Flag of reading block.
     *                   When there are not unread data at reading data,
     *                   set whether to block data until receiving the next 
     *                   data. (The default value:false)
     * @param write_block Flag of writing block.
     *                    If the buffer was full at writing data, set whether 
     *                    to block data until the buffer has space. 
     *                    (The default value:false)
     * @param read_timeout Data reading timeout time (millisecond) 
     *                     when not specifying read blocking.
     *                     (The default value:0)
     * @param write_timeout Data writing timeout time (millisecond)
     *                      when not specifying writing block.
     *                      (The default value:0)
     *
     * @endif
     */
    InPort(const char* name, DataType& value,
	   int bufsize=64, 
	   bool read_block = false, bool write_block = false,
	   int read_timeout = 0, int write_timeout = 0)
      :	InPortBase(name, toTypename<DataType>()),
        m_name(name), m_value(value),
	m_OnRead(NULL),  m_OnReadConvert(NULL),
        m_status(1) // InPort has only one status
    {
    }
    
    /*!
     * @if jp
     *
     * @brief $B%G%9%H%i%/%?(B
     *
     * $B%G%9%H%i%/%?!#(B
     *
     * @else
     *
     * @brief Destructor
     *
     * Destructor
     *
     * @endif
     */
    virtual ~InPort(void){};

    /*!
     * @if jp
     *
     * @brief $B%]!<%HL>>N$r<hF@$9$k!#(B
     *
     * $B%]!<%HL>>N$r<hF@$9$k!#(B
     *
     * @return $B%]!<%HL>>N(B
     *
     * @else
     *
     * @brief Get port name
     *
     * Get port name.
     *
     * @return The port name
     *
     * @endif
     */
    virtual const char* name()
    {
      return m_name.c_str();
    }

    
    /*!
     * @if jp
     *
     * @brief $B:G?7%G!<%?$,B8:_$9$k$+3NG'$9$k(B
     * 
     * InPort$B$KL$FI$N:G?7%G!<%?$,E~Ce$7$F$$$k$+$r(Bbool$BCM$GJV$9!#(B
     * InPort$B$,L$@\B3$N>l9g!"$*$h$S@\B3%3%M%/%?$N%P%C%U%!$,(BEmpty
     * $B$N>l9g$K$O(Bfalse$B$rJV$9!#(B
     *
     * @return true $BL$FI$N:G?7%G!<%?$,B8:_$9$k(B
     *         false $BL$@\B3$^$?$O%P%C%U%!$K%G!<%?$,B8:_$7$J$$!#(B
     * 
     * @else
     *
     * @brief Check whether the data is newest
     * 
     * Check whether the data stored at a current buffer position is newest.
     *
     * @return Newest data check result
     *         ( true:Newest data. Data has not been readout yet.
     *          false:Past data$B!%(BData has already been readout.)
     * 
     * @endif
     */
    virtual bool isNew()
    {
      RTC_TRACE(("isNew()"));
      int r(0);

      {
        Guard guard(m_connectorsMutex);
        if (m_connectors.size() == 0)
         {
            RTC_DEBUG(("no connectors"));
            return false;
         }
        r = m_connectors[0]->getBuffer()->readable();
      }

      if (r > 0)
        {
          RTC_DEBUG(("isNew() = true, readable data: %d", r));
          return true;
        }
      
      RTC_DEBUG(("isNew() = false, no readable data"));
      return false;
    }

    /*!
     * @if jp
     *
     * @brief $B%P%C%U%!$,6u$+$I$&$+3NG'$9$k(B
     * 
     * InPort$B$N%P%C%U%!$,6u$+$I$&$+$r(B bool $BCM$GJV$9!#(B
     * $B6u$N>l9g$O(B true, $BL$FI%G!<%?$,$"$k>l9g$O(B false $B$rJV$9!#(B
     *
     * @return true  $B%P%C%U%!$O6u(B
     *         false $B%P%C%U%!$KL$FI%G!<%?$,$"$k(B
     * 
     * @else
     *
     * @brief Check whether the data is newest
     * 
     * Check whether the data stored at a current buffer position is newest.
     *
     * @return Newest data check result
     *         ( true:Newest data. Data has not been readout yet.
     *          false:Past data$B!%(BData has already been readout.)
     * 
     * @endif
     */
    virtual bool isEmpty()
    {
      RTC_TRACE(("isEmpty()"));
      int r(0);

      {
        Guard guard(m_connectorsMutex);
        if (m_connectors.size() == 0)
          {
            RTC_DEBUG(("no connectors"));
            return true;
          }
        r = m_connectors[0]->getBuffer()->readable();
      }

      if (r == 0)
        {
          RTC_DEBUG(("isEmpty() = true, buffer is empty"));
          return true;
        }
      
      RTC_DEBUG(("isEmpty() = false, data exists in the buffer"));
      return false;
    }

    virtual int getUsedBufferLen()
    {
      RTC_TRACE(("getUsedBufferLen()"));
      int r(0);

      {
        Guard guard(m_connectorsMutex);
        if (m_connectors.size() == 0)
          {
            RTC_DEBUG(("no connectors"));
            return -1;
          }
        r = m_connectors[0]->getBuffer()->readable();
      }

      return r;
    }

    /*!
     * @if jp
     *
     * @brief DataPort $B$+$iCM$rFI$_=P$9(B
     *
     * InPort$B$K=q$-9~$^$l$?%G!<%?$rFI$_$@$9!#@\B3?t$,(B0$B!"$^$?$O%P%C%U%!$K(B
     * $B%G!<%?$,=q$-9~$^$l$F$$$J$$>uBV$GFI$_$@$7$?>l9g$NLa$jCM$OITDj$G$"$k!#(B
     * $B%P%C%U%!$,6u$N>uBV$N$H$-!"(B
     * $B;vA0$K@_Dj$5$l$?%b!<%I(B (readback, do_nothing, block) $B$K1~$8$F!"(B
     * $B0J2<$N$h$&$JF0:n$r$9$k!#(B
     *
     * - readback: $B:G8e$NCM$rFI$_$J$*$9!#(B
     *
     * - do_nothing: $B2?$b$7$J$$(B
     *
     * - block: $B%V%m%C%/$9$k!#%?%$%`%"%&%H$,@_Dj$5$l$F$$$k>l9g$O!"(B
     *       $B%?%$%`%"%&%H$9$k$^$GBT$D!#(B
     *
     * $B%P%C%U%!$,6u$N>uBV$G$O!"(BInPort$B$K%P%$%s%I$5$l$?JQ?t$NCM$,JV$5$l$k!#(B
     * $B$7$?$,$C$F!"=i2sFI$_=P$7;~$K$OITDjCM$rJV$92DG=@-$,$"$k!#(B
     * $B$3$N4X?t$rMxMQ$9$k:]$K$O!"(B
     *
     * - isNew(), isEmpty() $B$HJ;MQ$7!";vA0$K%P%C%U%!>uBV$r%A%'%C%/$9$k!#(B
     * 
     * - $B=i2sFI$_=P$7;~$KITDjCM$rJV$5$J$$$h$&$K%P%$%s%IJQ?t$r;vA0$K=i4|2=$9$k(B
     * 
     * - ReturnCode read(DataType& data) $B4X?t$NMxMQ$r8!F$$9$k!#(B
     *
     * $B$3$H$,K>$^$7$$!#(B
     *
     * $B3F%3!<%k%P%C%/4X?t$O0J2<$N$h$&$K8F$S=P$5$l$k!#(B
     * - OnRead: read() $B4X?t$,8F$P$l$k:]$KI,$:8F$P$l$k!#(B
     * 
     * - OnReadConvert: $B%G!<%?$NFI$_=P$7$,@.8y$7$?>l9g!"FI$_$@$7$?%G!<%?$r(B
     *       $B0z?t$H$7$F(BOnReadConvert$B$,8F$S=P$5$l!"La$jCM$r(Bread()$B$,La$jCM(B
     *       $B$H$7$FJV$9!#(B
     *
     * - OnEmpty: $B%P%C%U%!$,6u$N$?$a%G!<%?$NFI$_=P$7$K<:GT$7$?>l9g8F$S=P$5$l$k!#(B
     *        OnEmpty $B$NLa$jCM$r(B read() $B$NLa$jCM$H$7$FJV$9!#(B
     *
     * - OnBufferTimeout: $B%G!<%?%U%m!<7?$,(BPush$B7?$N>l9g$K!"FI$_=P$7(B
     *        $B%?%$%`%"%&%H$N$?$a$K%G!<%?$NFI$_=P$7$K<:GT$7$?>l9g$K8F$P$l$k!#(B
     *
     * - OnRecvTimeout: $B%G!<%?%U%m!<7?$,(BPull$B7?$N>l9g$K!"FI$_=P$7%?%$%`%"%&%H(B
     *        $B$N$?$a$K%G!<%?FI$_=P$7$K<:GT$7$?>l9g$K8F$P$l$k!#(B
     *
     * - OnReadError: $B>e5-0J30$NM}M3$GFI$_$@$7$K<:GT$7$?>l9g$K8F$P$l$k!#(B
     *        $BM}M3$H$7$F$O!"%P%C%U%!@_Dj$NIT@09g!"Nc30$NH/@8$J$I$,9M$($i$l$k(B
     *        $B$,DL>o$O5/$3$j$($J$$$?$a%P%0$N2DG=@-$,$"$k!#(B
     *
     * @return $BFI$_=P$77k2L(B($BFI$_=P$7@.8y(B:true, $BFI$_=P$7<:GT(B:false)
     *
     * @else
     *
     * @brief Readout the value from DataPort
     *
     * Readout the value from DataPort
     *
     * - When Callback functor OnRead is already set, OnRead will be invoked
     *   before reading from the buffer held by DataPort.
     * - When the buffer held by DataPort can detect the underflow,
     *   and when it detected the underflow at reading, callback functor
     *   OnUnderflow will be invoked.
     * - When callback functor OnReadConvert is already set, the return value of
     *   operator() of OnReadConvert will be the return value of read().
     * - When timeout of reading is already set by setReadTimeout(),
     *   it waits for only timeout time until the state of the buffer underflow
     *   is reset, and if OnUnderflow is already set, this will be invoked to 
     *   return.
     *
     * @return Readout result (Successful:true, Failed:false)
     *
     * @endif
     */
    bool read()
    {
      RTC_TRACE(("DataType read()"));

      if (m_OnRead != NULL) 
        {
          (*m_OnRead)();
          RTC_TRACE(("OnRead called"));
        }

      cdrMemoryStream cdr;
      ReturnCode ret;
      {
        Guard guard(m_connectorsMutex);
        if (m_connectors.size() == 0)
          {
            RTC_DEBUG(("no connectors"));
            return false;
          }

        ret = m_connectors[0]->read(cdr);
        m_status[0] = ret;
      }
      if (ret == PORT_OK)
        {
          RTC_DEBUG(("data read succeeded"));
          m_value <<= cdr;
          if (m_OnReadConvert != 0) 
            {
              m_value = (*m_OnReadConvert)(m_value);
              RTC_DEBUG(("OnReadConvert called"));
              return true;
            }
          return true;
        }
      else if (ret == BUFFER_EMPTY)
        {
          RTC_WARN(("buffer empty"));
          return false;
        }
      else if (ret == BUFFER_TIMEOUT)
        {
          RTC_WARN(("buffer read timeout"));
          return false;
        }
      RTC_ERROR(("unknown retern value from buffer.read()"));
      return false;
    }
    

    /*!
     * @if jp
     *
     * @brief $B%P%$%s%I$5$l$?(B T $B7?$NJQ?t$K(B InPort $B%P%C%U%!$N:G?7CM$rFI$_9~$`(B
     *
     * $B%P%$%s%I$5$l$?(B T $B7?$N%G!<%?$K(B InPort $B$N:G?7CM$rFI$_9~$`!#(B
     * $B%3%s%9%H%i%/%?$G(B T $B7?$NJQ?t$H(B InPort $B$,%P%$%s%I$5$l$F$$$J$1$l$P$J$i$J$$!#(B
     * $B$3$N%a%=%C%I$O%]%j%b!<%U%#%C%/$K;HMQ$5$l$k;v$rA0Ds$H$7$F$$$k$?$a!"(B
     * $B7?$K0MB8$7$J$$0z?t!"La$jCM$H$J$C$F$$$k!#(B
     *
     * @else
     *
     * @brief Read the newly value to type-T variable which is bound to InPort's
     *        buffer.
     *
     * Read the newly value to type-T data which is bound to InPort's buffer.
     * The type-T variable must be bound to InPort in constructor.
     * Since this method assumes to be used for polymorphic,
     * its argument and the return value do not depend on type.
     *
     * @endif
     */
    virtual void update()
    {
      this->read();
    };
    
    /*!
     * @if jp
     *
     * @brief T $B7?$N%G!<%?$X(B InPort $B$N:G?7CM%G!<%?$rFI$_9~$`(B
     *
     * InPort $B$K@_Dj$5$l$F$$$k:G?7%G!<%?$rFI$_9~$_!"(B
     * $B;XDj$5$l$?%G!<%?JQ?t$K@_Dj$9$k!#(B
     *
     * @param rhs InPort $B%P%C%U%!$+$iCM$rFI$_9~$`(B T $B7?JQ?t(B
     *
     * @else
     *
     * @brief Read the newly value data in InPort to type-T variable
     *
     * Read the newly data set in InPort and set to specified data variable.
     *
     * @param rhs The type-T variable to read from InPort's buffer
     *
     * @endif
     */
    void operator>>(DataType& rhs)
    {
      this->read();
      rhs = m_value;
      return;
    }
    
    /*!
     * @if jp
     *
     * @brief $B%3%M%/%?$+$i$NFI$_=P$7%9%F!<%?%9$rF@$k(B
     *
     * $B$3$N4X?t$O!"(BDataPortStatus $B$N(B vector $B7?$N(B 0 $BHVL\$NMWAG$K!"FI$_=P(B
     * $B$7$N:]$N%9%F!<%?%9$rJV$9!#0z?t(B index $B$O8_49@-$N$?$a$@$1$KMQ0U$5(B
     * $B$l$F$*$jFbItE*$K$O;HMQ$5$l$J$$!#(B
     * 
     * InPort $B$OM#0l$N%P%C%U%!$r;}$A!"@\B3$4$H$K@8@.$5$l$k2>A[%G!<%?%A%c(B
     * $B%M%k(B Connector $B$O!"$3$NM#0l$N%P%C%U%!$r6&M-$9$k!#$7$?$,$C$F!"(B
     * read() $B4X?t$O(B 0 $BHVL\$N(B Connector $B$+$i$N$_%G!<%?$NFI$_=P$7$r9T$&!#(B
     *
     * InPort $B$N(B read() $B4X?t$G$O!"(BConnector $B$+$i$N%G!<%?FI$_=P$7$,@5>o(B
     * $B=*N;$7$?$H$-$N$_(B true $B$rJV$7!"$=$l0J30$G$O(B false $B$rJV5Q$9$k!#$3(B
     * $B$N4X?t$O(B read() $B$,(B false $B$N>l9g%9%F!<%?%9$rD4$Y$k$N$K;HMQ$9$k$3(B
     * $B$H$,$G$-$k!#(B
     * 
     * @param index Connector $B$N(B index
     * @return $B%9%F!<%?%9(B
     *
     * @else
     *
     * @brief Getting specified connector's reading status
     *
     * This function returns read status as a 0th element of the
     * vector of the DataPortStatus. Argument "index" is provided for
     * only compatibility, and it is not used internally.
     *
     * InPort has only one buffer, and connectors, which are created
     * for each connection as virtual data stream channel, share the
     * buffer. Therefore read() function always read data from the
     * first Connector.
     *
     * InPort returns true-value when read() function normally
     * finishes, and in other cases it returns false-value. This
     * function is used to check return status when read() returns
     * false-value.
     *
     * @param index Connector index (no effect)
     * @return Reading status
     *
     * @endif
     */
    DataPortStatus::Enum getStatus(int index)
    {
      return m_status[0];
    }

    /*!
     * @if jp
     *
     * @brief $B%3%M%/%?$+$i$NFI$_=P$7%9%F!<%?%9$rF@$k(B
     *
     * $B$3$N4X?t$O!">o$KD9$5(B1$B$N(BDataPortStatusList$B7?$NFI$_=P$7$N:]$N%9%F!<(B
     * $B%?%9$rJV$9!#(B
     * 
     * InPort $B$OM#0l$N%P%C%U%!$r;}$A!"@\B3$4$H$K@8@.$5$l$k2>A[%G!<%?%A%c(B
     * $B%M%k(B Connector $B$O!"$3$NM#0l$N%P%C%U%!$r6&M-$9$k!#$7$?$,$C$F!"(B
     * read() $B4X?t$O(B 0 $BHVL\$N(B Connector $B$+$i$N$_%G!<%?$NFI$_=P$7$r9T$&!#(B
     *
     * InPort $B$N(B read() $B4X?t$G$O!"(BConnector $B$+$i$N%G!<%?FI$_=P$7$,@5>o(B
     * $B=*N;$7$?$H$-$N$_(B true $B$rJV$7!"$=$l0J30$G$O(B false $B$rJV5Q$9$k!#$3(B
     * $B$N4X?t$O(B read() $B$,(B false $B$N>l9g%9%F!<%?%9$rD4$Y$k$N$K;HMQ$9$k$3(B
     * $B$H$,$G$-$k!#(B
     * 
     * @return $B%9%F!<%?%9(B
     *
     * @else
     *
     * @brief Getting specified connector's reading status
     *
     * This function always returns reading status by
     * DataPortStatusList value with length 1.
     *
     * InPort has only one buffer, and connectors, which are created
     * for each connection as virtual data stream channel, share the
     * buffer. Therefore read() function always read data from the
     * first Connector.
     *
     * InPort returns true-value when read() function normally
     * finishes, and in other cases it returns false-value. This
     * function is used to check return status when read() returns
     * false-value.
     *
     * @return Writing status
     *
     * @endif
     */
    DataPortStatusList getStatusList()
    {
      return m_status;
    }

    /*!
     * @if jp
     *
     * @brief InPort $B%P%C%U%!$X%G!<%?FI$_9~$_;~$N%3!<%k%P%C%/$N@_Dj(B
     *
     * InPort $B$,;}$D%P%C%U%!$+$i%G!<%?$,FI$_9~$^$l$kD>A0$K8F$P$l$k%3!<%k%P%C%/(B
     * $B%*%V%8%'%/%H$r@_Dj$9$k!#(B
     * 
     * @param on_read OnRead&lt;DataType&gt;$B7?$N%*%V%8%'%/%H(B
     *
     * @else
     *
     * @brief Set callback when data is read from the InPort buffer
     *
     * Set the callback object that is invoked right before data is read from 
     * the InPort's buffer
     * 
     * @param on_read OnRead&lt;DataType&gt; type object
     *
     * @endif
     */
    inline void setOnRead(OnRead<DataType>* on_read)
    {
      m_OnRead = on_read;
    }
    
    /*!
     * @if jp
     *
     * @brief InPort $B%P%C%U%!$X%G!<%?FI$_=P$7;~$N%3!<%k%P%C%/$N@_Dj(B
     *
     * InPort $B$,;}$D%P%C%U%!$+$i%G!<%?$,FI$_=P$5$l$k:]$K8F$P$l$k%3!<%k%P%C%/(B
     * $B%*%V%8%'%/%H$r@_Dj$9$k!#%3!<%k%P%C%/%*%V%8%'%/%H$NLa$jCM$,(Bread()$B%a%=%C%I(B
     * $B$N8F=P7k2L$H$J$k!#(B
     * 
     * @param on_rconvert OnReadConvert&lt;DataType&gt;$B7?$N%*%V%8%'%/%H(B
     *
     * @else
     *
     * @brief Set callback when data is readout to the InPort buffer
     *
     * Set the callback object that is invoked when data is readout to
     * the InPort's buffer. The return value of callback object is the return
     * result of the read() method.
     * 
     * @param on_rconvert OnReadConvert&lt;DataType&gt; type object
     *
     * @endif
     */
    inline void setOnReadConvert(OnReadConvert<DataType>* on_rconvert)
    {
      m_OnReadConvert = on_rconvert;
    }
    
  private:
    std::string m_typename;
    /*!
     * @if jp
     * @brief $B%]!<%HL>(B
     * @else
     * @berif Port's name
     * @endif
     */
    std::string m_name;
    
    /*!
     * @if jp
     * @brief $B%P%$%s%I$5$l$k(B T $B7?$NJQ?t$X$N;2>H(B
     * @else
     * @brief The reference to type-T value bound this OutPort
     * @endif
     */
    DataType& m_value;
    
    /*!
     * @if jp
     * @brief OnRead $B%3!<%k%P%C%/%U%!%s%/%?$X$N%]%$%s%?(B
     * @else
     * @brief Pointer to OnRead callback functor
     * @endif
     */
    OnRead<DataType>* m_OnRead;
    
    /*!
     * @if jp
     * @brief OnReadConvert $B%3!<%k%P%C%/%U%!%s%/%?$X$N%]%$%s%?(B
     * @else
     * @brief Pointer to OnReadConvert callback functor
     * @endif
     */
    OnReadConvert<DataType>* m_OnReadConvert;

    DataPortStatusList m_status;
  };
}; // End of namesepace RTM

#endif // RTC_INPORT_H
