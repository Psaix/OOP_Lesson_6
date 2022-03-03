#include <iostream>
#include <vector>




class Card
{
public:
    enum rank {
        ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
        JACK, QUEEN, KING
    };
    enum suit { CLUBS, DIAMONDS, HEARTS, SPADES };

    Card(rank r = ACE, suit s = SPADES, bool ifu = true);
    int GetValue() const;
    void Flip();

private:
    rank m_Rank;
    suit m_Suit;
    bool m_IsFaceUp;



//____________________________________________________________________________________________________________
//____________________________________________TASK3___________________________________________________________
//__________________________________________OPERATOR CARD_____________________________________________________


    friend std::ostream& operator<<(std::ostream& os, const Card& aCard)
    {
        const std::string RANKS[] = { "0", "A", "2", "3", "4",
                                       "5", "6", "7", "8", "9",
                                      "10", "J", "Q", "K" };
        const std::string SUITS[] = { "c", "d", "h", "s" };

        if (aCard.m_IsFaceUp) { os << RANKS[aCard.m_Rank] << SUITS[aCard.m_Suit]; }
        else { os << "XX"; }
        return os;
    }
};


Card::Card(rank r, suit s, bool ifu) : m_Rank(r), m_Suit(s), m_IsFaceUp(ifu)
{}

int Card::GetValue() const
{
    int value = 0;
    if (m_IsFaceUp)
    {
        value = m_Rank;
        if (value > 10)
        {
            value = 10;
        }
    }
    return value;
}

void Card::Flip()
{
    m_IsFaceUp = !(m_IsFaceUp);
}




class Hand
{
public:
    Hand();

    virtual ~Hand();

    void Add(Card* pCard);

    void Clear();

    int GetTotal() const;

protected:
    std::vector<Card*> m_Cards;
};

Hand::Hand()
{
    m_Cards.reserve(7);
}

Hand::~Hand()
{
    Clear();
}

void Hand::Add(Card* pCard)
{
    m_Cards.push_back(pCard);
}

void Hand::Clear()
{
    std::vector<Card*>::iterator iter = m_Cards.begin();
    for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
    {
        delete* iter;
        *iter = 0;
    }

    m_Cards.clear();
}

int Hand::GetTotal() const
{
    if (m_Cards.empty())
    {
        return 0;
    }

    if (m_Cards[0]->GetValue() == 0)
    {
        return 0;
    }

    int total = 0;
    std::vector<Card*>::const_iterator iter;
    for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
    {
        total += (*iter)->GetValue();
    }

    bool containsAce = false;
    for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
    {
        if ((*iter)->GetValue() == Card::ACE)
        {
            containsAce = true;
        }
    }

    if (containsAce && total <= 11)
    {

        total += 10;
    }

    return total;
}


class GenericPlayer : public Hand
{
    friend std::ostream& operator<<(std::ostream& os, const GenericPlayer& aGenericPlayer);

public:
    GenericPlayer(const std::string& name = "");

    virtual ~GenericPlayer();

    virtual bool IsHitting() const = 0;

    bool IsBoosted() const;

    void Bust() const;

protected:
    std::string m_Name;
};


bool GenericPlayer::IsBoosted() const
{
    return (GetTotal() > 21);
}


void GenericPlayer::Bust() const
{
    std::cout << m_Name << " busts.\n";
}


std::ostream& operator<<(std::ostream& os, const GenericPlayer& aGenericPlayer)
{
    os << aGenericPlayer.m_Name << ":\t";

    std::vector<Card*>::const_iterator pCard;
    if (!aGenericPlayer.m_Cards.empty())
    {
        for (pCard = aGenericPlayer.m_Cards.begin();
            pCard != aGenericPlayer.m_Cards.end();
            ++pCard)
        {
            os << (*pCard) << "\t";
        }


        if (aGenericPlayer.GetTotal() != 0)
        {
            std::cout << "(" << aGenericPlayer.GetTotal() << ")";
        }
    }
    else
    {
        os << "<empty>";
    }

    return os;
}


//____________________________________________________________________________________________________________
//____________________________________________TASK3___________________________________________________________
//___________________________________________PLAYER__________________________________________________________

class Player : public GenericPlayer
{
public:
    Player(const std::string& name = "");

    virtual ~Player();

    virtual bool IsHitting() const;

    void Win() const;

    void Lose() const;

    void Push() const;
};


bool Player::IsHitting() const
{
    std::cout << m_Name << ", do you want a hit? (Y/N): ";
    char response;
    std::cin >> response;
    return (response == 'y' || response == 'Y');
}


void Player::Win() const
{
    std::cout << m_Name << " wins.\n";
}

void Player::Lose() const
{
    std::cout << m_Name << " loses.\n";
}

void Player::Push() const
{
    std::cout << m_Name << " pushes.\n";
}



//____________________________________________________________________________________________________________
//____________________________________________TASK4___________________________________________________________
//____________________________________________HOUSE__________________________________________________________

class House : public GenericPlayer
{
public:
    House(const std::string& name = "House");

    virtual ~House();

    virtual bool IsHitting() const;

    void FlipFirstCard();
};

bool House::IsHitting() const
{
    return (GetTotal() <= 16);
}


void House::FlipFirstCard()
{
    if (!(m_Cards.empty()))
    {
        m_Cards[0]->Flip();
    }
    else
    {
        std::cout << "No card to flip!\n";
    }
}
