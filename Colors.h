struct rgba_color
{
    float red, green, blue, alpha;

    bool operator==(const rgba_color& rhs)
    {
        if(this->blue == rhs.blue && this->green == rhs.green && this->red == rhs.red && this->alpha == rhs.alpha)
        {
            return true;
        }

        return false;
    }

    bool operator!=(const rgba_color& rhs)
    {
        return !(*this == rhs);
    }
};

const rgba_color RED {1, 0, 0, 1};
const rgba_color GREEN {0, 1, 0, 1};
const rgba_color BLUE {0, 0, 1, 1};
const rgba_color WHITE {1, 1, 1, 1};
const rgba_color BLACK {0, 0, 0, 1};
const rgba_color PURPLE {1, 0, 1, 1};
const rgba_color YELLOW {1, 1, 0, 1};
const rgba_color ORANGE {1, 0.5, 0, 1};