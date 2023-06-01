int ToMilliseconds(int Value, int Unit)
{
    if (Unit == 0)
    {
        return (int)(1000 / Value);
    }

    if (Unit == 1)
    {
        return (int)(1000 * 60 / Value);
    }

    if (Unit == 2)
    {
        return (int)(1000 * 60 * 60 / Value);
    }
}