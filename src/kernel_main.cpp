
extern "C" void main()
{
    const char* greeting = "Hello, World!\0";


    *(char*)0xb8000 = 'R';
    char* pointer = (char*)0xb8000;
    int i = 0;
    while (greeting[i] != '\0')
    {
        pointer[i*2] = greeting[i];
        i++;
    }
    return;
}