#include<vector>

class AudioProcessor {
public:
    //compress the audio against given threshold
    std::vector<short> Compress(const std::vector<short>& audio, short threshold, float rate);
    //cut out silence part of given length
    std::vector<short> CutOutSilence(const std::vector<short>& audio, short level, int silenceLength);
    //stretch the audio twice
    std::vector<short> StretchTwice(const std::vector<short>& audio);
    //normalize the samples under given target
    std::vector<short> Normalize(const std::vector<short>& audio, short normalizeTarget);
};