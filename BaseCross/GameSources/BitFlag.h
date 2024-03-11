#pragma once
using namespace std; // STDの省略

/*!
@brief ビットフラグ用継承元クラス
@param フラグ指定子用enum
@param bit計算する型
*/
template <class Enum, class BitType>
class BitFlag
{
protected:

    static_assert(is_enum<Enum>(), "変数宣言時にちゃんとenum入れてますか？");

    BitType m_flags; // フラグ保存先

    /*!
    @brief コンストラクタ
    */
    BitFlag(BitType initialValue = 0) : m_flags(initialValue) {}

    /*!
    @brief ビットシフト関数
    @param enum
    @return BitType
    */
    BitType BitShift(Enum flag) const
    {
        return static_cast<BitType>(1) << static_cast<BitType>(flag);
    }

    /*!
    @brief フラグがオンになってるかの取得関数
    @return オンだったらtrue、オフだったらfalse
    */
    bool isFlag(Enum flag) const
    {
        return (m_flags & BitShift(flag)) != 0;
    }

    /*!
    @brief フラグ設定用クラス
    */
    class SetResult
    {
    private:

        BitFlag& parent; // 呼び出し先
        BitType value; // 変更するビット

    public:

        /*!
        @brief コンストラクタ
        @param フラグ管理クラス
        @param 変更するビット
        */
        SetResult(BitFlag& parent, BitType value) : parent(parent), value(value) {}

        /*!
        @brief Set(enum) = boolで設定出来る様にする為の=演算子のオーバーロード
        @param bool
        */
        void operator=(bool boolen)
        {
            // 真偽に応じてフラグをオンオフ
            if (boolen) 
            {
                parent.m_flags |= value;
            }
            else 
            {
                parent.m_flags &= ~value;
            }
        }
    };

public:

    ///*!
    //@brief フラグをオンにする関数
    //@param enum
    //*/
    //void SetFlag(Enum flag)
    //{
    //    // ビットシフトしたenumの位置のビットをオア演算でオンにする
    //    m_flags |= BitShift(flag);
    //}

    ///*!
    //@brief フラグをオンにする関数
    //@param enum(可変長)
    //*/
    //template<class... Enums>
    //void SetFlag(Enums... flags)
    //{
    //    // 可変長引数の数だけループさせる
    //    for (Enum flag : { static_cast<Enum>(flags)... })
    //    {
    //        SetFlag(flag);
    //    }
    //}

    ///*!
    //@brief フラグをオフにする関数
    //@param enum
    //*/
    //void ClearFlag(Enum flag)
    //{
    //    // ビットシフトしたenumの位置のビットをノット演算でオフにする
    //    m_flags &= ~BitShift(flag);
    //}

    ///*!
    //@brief フラグをオフにする関数
    //@param enum(可変長)
    //*/
    //template<class... Enums>
    //void ClearFlag(Enums... flags)
    //{
    //    // 可変長引数の数だけループさせる
    //    for (Enum flag : { static_cast<Enum>(flags)... })
    //    {
    //        ClearFlag(flag);
    //    }
    //}

    /*!
    @brief フラグがオンになってるかの取得関数
    @param enum(可変長)
    */
    template<class... Enums>
    bool operator()(Enums... flags) const
    {
        bool result = true; // 保存用変数

        // 可変長引数の数だけループさせる
        for (Enum flag : { static_cast<Enum>(flags)... })
        {
            result = result && isFlag(flag);
        }
        return result;
    }

    /*!
    @brief Set(enum) = boolで設定出来る様にする為の()演算子のオーバーロード
    @param enum(可変長)
    */
    template<class... Enums>
    SetResult Set(Enums... flags)
    {
        BitType temp = 0; // 保存用変数

        // 可変長引数の数だけループさせる
        for (Enum flag : { static_cast<Enum>(flags)... })
        {
            temp |= BitShift(flag);
        }

        // 設定用クラスを生成して返す
        return SetResult(*this, temp);
    }

    /*!
    @brief リセット関数
    */
    void Reset()
    {
        m_flags = 0;
    }
};