/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: datetime.h,v 1.13 2008-10-07 11:06:26 tat Exp $
 ***************************************************************************/
#ifndef _MIMETIC_RFC822_DATETIME_H
#define _MIMETIC_RFC822_DATETIME_H
#include <string>
#include <iostream>
#include <mimetic/strutils.h>
#include <mimetic/rfc822/fieldvalue.h>

namespace mimetic
{


/// RFC822 DateTime field representation
struct DateTime: public FieldValue
{
    struct DayOfWeek {
        enum DayName { mnShort = 0, mnLong = 1 };
        DayOfWeek(int iDayOfWeek);
        DayOfWeek(const std::string&);
        bool operator==(const std::string&);
        bool operator==(int iDayOfWeek);
        std::string name(bool longName = false) const;
        short ordinal() const;
    private:
        static const char *ms_label[][2];
        short m_iDayOfWeek;
    };
    struct Month {
        enum MonthName { mnShort = 0, mnLong = 1 };
        Month(int iMonth);
        Month(const std::string& );
        bool operator==(const std::string& ) const;
        bool operator==(int iMonth) const;
        std::string name(bool longName = false) const;
        short ordinal() const;
    private:
        static const char *ms_label[][2];
        short m_iMonth;
    };
    struct Zone {
        Zone(int iZone);
        Zone(const std::string& );
        bool operator==(const std::string&);
        bool operator==(int iZone);
        std::string name() const;
        short ordinal() const;
    private:
        static int ms_offset[];
        static const char *ms_label[];
        short m_iZone, m_iZoneIdx;
        std::string m_sZone;
    };

    // DateTime
    enum {
        Jan = 1, Feb, Mar, Apr, May, Jun, Jul,
        Aug, Sep, Oct, Nov, Dec
    };
    enum {
        Mon = 1, Tue, Wed, Thu, Fri, Sat, Sun
    };

    enum {
        GMT    = +000,
        UT     = +000,
        BST     = +100,
        CET    = +100,
        MET    = +100,
        EET    = +200,
        IST    = +200,
        METDST= +200,
        EDT    = -400,
        CDT    = -500,
        EST    = -500,
        CST    = -600,
        MDT    = -600,
        MST    = -700,
        PDT    = -700,
        HKT    = +800,
        PST    = -800,
        JST    = +900
    };
    DateTime();
    DateTime(const char*);
    DateTime(const std::string&);
    DayOfWeek dayOfWeek() const;
    short day() const;
    Month month() const;
    short year() const;
    short hour() const;
    short minute() const;
    short second() const;
    Zone zone() const;
    std::string str() const;
    friend std::ostream& operator<<(std::ostream&, const DateTime&);
protected:
    FieldValue* clone() const;
private:
    void set(const std::string&);
    mutable int m_iDayOfWeek;
    int m_iDay, m_iMonth, m_iYear;
    int m_iHour, m_iMinute, m_iSecond;
    std::string m_zone;
};


}

#endif
