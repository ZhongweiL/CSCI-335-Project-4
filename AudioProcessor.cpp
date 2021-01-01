#include"AudioProcessor.hpp"
#include<vector>
#include<cmath>
#include<stdexcept>
#include<algorithm>

//compress the audio against given threshold
std::vector<short> AudioProcessor::Compress(const std::vector<short>& audio, short threshold, float rate) {
    //throw exception on invalid argument
    if (threshold < 0 || rate < 1.0) {
        throw std::invalid_argument("invalid threshold or rate");
    }
    //compress the samples and push them into the a new vector
    std::vector<short> newAudio;
    for (short sample: audio) {
        //case when the sample is under the threshold
        if (sample <= threshold && -sample <= threshold) {
            newAudio.push_back(sample);
        } else {
            //cases for both positive and negative numbers that exceeds the threshold
            short newSample = (sample >= 0) ? round((sample - threshold) / rate + threshold): -round((-sample - threshold) / rate + threshold);
            newAudio.push_back(newSample);
        }
    }
    return newAudio;
}

//cut out silence part of given length
std::vector<short> AudioProcessor::CutOutSilence(const std::vector<short>& audio, short level, int silenceLength) {
    //throw exception on invalid argument
    if (level < 0 || silenceLength < 1) throw std::invalid_argument("invalid level or silence length");
    //return the audio if its size is less than silence length
    if (audio.size() < silenceLength) return audio;
    std::vector<short> newAudio;
    //use double pointers to keep track of the silence length (i and behind)
    int behind = 0;
    for (size_t i = 0; i < audio.size(); i++) {
        if (audio[i] <= level && audio[i] >= -level) continue;
        //push all elements between the pointers if their distance is shorter than the silence length
        if (behind < i && i - behind < silenceLength) {
            while (behind < i) {
                newAudio.push_back(audio[behind++]);
            }
        }
        newAudio.push_back(audio[i]);
        //bring behind to the same position as i and increment it
        behind = i + 1;
    }
    //case when the last sample is in silent (the last portion isn't taken into consideration)
    if (behind < audio.size() && audio.size() - behind < silenceLength) {
        while (behind < audio.size()) {
            newAudio.push_back(audio[behind++]);
        }
    }
    return newAudio;
}

//stretch the audio twice
std::vector<short> AudioProcessor::StretchTwice(const std::vector<short>& audio) {
    //return the same audio if the audio cannot be stretched
    if (audio.size() < 2) return audio;
    std::vector<short> newAudio;
    for (size_t i = 0; i < audio.size()-1; i++) {
        newAudio.push_back(audio[i]);
        newAudio.push_back(round((audio[i] + audio[i+1]) / 2.0));
    }
    newAudio.push_back(audio[audio.size()-1]);
    return newAudio;
}

//normalize the samples under given target
std::vector<short> AudioProcessor::Normalize(const std::vector<short>& audio, short normalizeTarget) {
    //throw exception on invalid normalize target
    if (normalizeTarget < 1) throw std::invalid_argument("normalize target cannot be less than 1");
    short largestAbs = 0;
    for (short sample: audio) {
        largestAbs = std::max(std::abs(sample), std::abs(largestAbs));
    }
    //return the same audio if all samples are zero or if the audio is empty
    if (largestAbs == 0) return audio;
    //calculate the rate and normalize the audio
    float rate = (float)normalizeTarget / (float)largestAbs;
    std::vector<short> newAudio;
    for (short sample: audio) {
        newAudio.push_back(round(sample * rate));
    }
    return newAudio;
}