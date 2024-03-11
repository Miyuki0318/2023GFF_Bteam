#pragma once
#include "BitFlag.h"

/*!
@brief ビットフラグ用クラス(8個まで:1バイト)
@param フラグ指定子用enum
*/
template <class Enum>
class Bool8_t : public BitFlag<Enum, uint8_t>
{
public:

    /*!
    @brief コンストラクタ
    @param 初期値 = 0
    */
    Bool8_t(uint8_t initialValue = 0) : BitFlag<Enum, uint8_t>(initialValue) {}
};

/*!
@brief ビットフラグ用クラス(16個まで:2バイト)
@param フラグ指定子用enum
*/
template <class Enum>
class Bool16_t : public BitFlag<Enum, uint16_t>
{
public:

    /*!
    @brief コンストラクタ
    @param 初期値 = 0
    */
    Bool16_t(uint16_t initialValue = 0) : BitFlag<Enum, uint16_t>(initialValue) {}
};

/*!
@brief ビットフラグ用クラス(32個まで:4バイト)
@param フラグ指定子用enum
*/
template <class Enum>
class Bool32_t : public BitFlag<Enum, uint32_t>
{
public:

    /*!
    @brief コンストラクタ
    @param 初期値 = 0
    */
    Bool32_t(uint32_t initialValue = 0) : BitFlag<Enum, uint32_t>(initialValue) {}
};

/*!
@brief ビットフラグ用クラス(64個まで:8バイト)
@param フラグ指定子用enum
*/
template <class Enum>
class Bool64_t : public BitFlag<Enum, uint64_t>
{
public:

    /*!
    @brief コンストラクタ
    @param 初期値 = 0
    */
    Bool64_t(uint64_t initialValue = 0) : BitFlag<Enum, uint64_t>(initialValue) {}
};