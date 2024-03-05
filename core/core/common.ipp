LONGLONG Timer::frecuency;
LONGLONG Timer::last;
LONGLONG Timer::curr;
float    Timer::delta;
float    Timer::timeScale;

void Timer::SetTimeScale(float param)
{
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frecuency));
    timeScale = param / static_cast<float>(frecuency);
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&last));
}

void Timer::Update()
{
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curr));
    delta = static_cast<float>(curr - last) * timeScale;
    last = curr;
}

float Timer::DeltaTime()
{
    return delta;
}
