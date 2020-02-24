

#include "GobFileLoader.h"
#include "assert.h"

namespace GobFileLoader {

    CGobFileLoader::CGobFileLoader(const std::string& aFilename)
    {
        std::ifstream lFileReader(aFilename, std::ios::binary);

        Header lHeader;
        uint32_t lChunkSize = sizeof Header;
        lFileReader.read(reinterpret_cast<char*>(&lHeader), lChunkSize);
        assert(memcmp(lHeader.mMagic, GOB_FILE_MAGIC, GOB_FILE_MAGIC_LEN) == 0 && lHeader.mVersion >= GOB_FILE_VERSION_PREV);

        mBoundingBox = lHeader.mBounds;

        box2 box = mBoundingBox;
        if (lHeader.mPointCount > 0)
        {
            lChunkSize = sizeof PointInstanceVertex * lHeader.mPointCount;
            mPoints.resize(lHeader.mPointCount);
            lFileReader.read(reinterpret_cast<char*>(mPoints.data()), lChunkSize);

            for (auto & p : mPoints) {
                points.push_back(new GlowPoint(p.normal(box.min, box.max, float(gpGlobal->GetConfiguration<AppConfiguration>().GetWidth()), float(gpGlobal->GetConfiguration<AppConfiguration>().GetHeight()))));
            }
        }

        if (lHeader.mLineCount > 0)
        {
            lChunkSize = sizeof LineInstanceVertex * lHeader.mLineCount;
            mLines.resize(lHeader.mLineCount);
            lFileReader.read(reinterpret_cast<char*>(mLines.data()), lChunkSize);
            for (auto& l : mLines) {
                lines.push_back(new line(l.normal(box.min, box.max, float(gpGlobal->GetConfiguration<AppConfiguration>().GetWidth()), float(gpGlobal->GetConfiguration<AppConfiguration>().GetHeight()))));
            }
    #ifdef HT01
                for (int i = 0; i < 2; ++i) {
                    lines.insert(lines.end(), lines.begin(), lines.end());
                }
                //lines.insert(lines.end(), lines.begin(), lines.begin() + lines.size() / 2);
    #endif
        }

        if (lHeader.mEllipseArcCount > 0)
        {
            lChunkSize = sizeof EllipseArcInstanceVertex * lHeader.mEllipseArcCount;
            mEllipseArcs.resize(lHeader.mEllipseArcCount);
            lFileReader.read(reinterpret_cast<char*>(mEllipseArcs.data()), lChunkSize);

            for (auto & ea : mEllipseArcs) {
                ellipses.push_back(new ellipse(ea.normal(box.min, box.max, float(gpGlobal->GetConfiguration<AppConfiguration>().GetWidth()), float(gpGlobal->GetConfiguration<AppConfiguration>().GetHeight()))));
            }
        }

        if (lHeader.mEllipseCount > 0)
        {
            lChunkSize = sizeof EllipseInstanceVertex * lHeader.mEllipseCount;
            mEllipses.resize(lHeader.mEllipseCount);
            lFileReader.read(reinterpret_cast<char*>(mEllipses.data()), lChunkSize);

            for (auto & e : mEllipses) {
                ellipses.push_back(new ellipse(e.normal(box.min, box.max, float(gpGlobal->GetConfiguration<AppConfiguration>().GetWidth()), float(gpGlobal->GetConfiguration<AppConfiguration>().GetHeight()))));
            }

    #ifdef HT01
                for (int i = 0; i < 2; ++i) {
                    ellipses.insert(ellipses.end(), ellipses.begin(), ellipses.end());
                }
                ellipses.insert(ellipses.end(), ellipses.begin(), ellipses.begin() + ellipses.size() / 2);
    #endif
        }


        if (lHeader.mCircleArcCount > 0)
        {
            lChunkSize = sizeof CircleArcInstanceVertex * lHeader.mCircleArcCount;
            mCircleArcs.resize(lHeader.mCircleArcCount);
            lFileReader.read(reinterpret_cast<char*>(mCircleArcs.data()), lChunkSize);
            for (auto& ca : mCircleArcs) {
                circles.push_back(new ellipse(ca.normal(box.min, box.max, float(gpGlobal->GetConfiguration<AppConfiguration>().GetWidth()), float(gpGlobal->GetConfiguration<AppConfiguration>().GetHeight()))));
            }
        }

        if (lHeader.mCircleCount > 0)
        {
            lChunkSize = sizeof CircleInstanceVertex * lHeader.mCircleCount;
            mCircles.resize(lHeader.mCircleCount);
            lFileReader.read(reinterpret_cast<char*>(mCircles.data()), lChunkSize);

            for (auto& c : mCircles) {
                circles.push_back(new ellipse(c.normal(box.min, box.max, float(gpGlobal->GetConfiguration<AppConfiguration>().GetWidth()), float(gpGlobal->GetConfiguration<AppConfiguration>().GetHeight()))));
            }

    #ifdef HT01
                for (int i = 0; i < 2; ++i) {
                    circles.insert(circles.end(), circles.begin(), circles.end());
                }
                circles.insert(circles.end(), circles.begin(), circles.begin() + circles.size() / 2);
    #endif
        }

        if (lHeader.mLineTypeCount > 0)
        {
            lChunkSize = sizeof LineTypeInfo * lHeader.mLineTypeCount;
            mLineTypes.resize(lHeader.mLineTypeCount);
            lFileReader.read(reinterpret_cast<char*>(mLineTypes.data()), lChunkSize);
        }
    }
}