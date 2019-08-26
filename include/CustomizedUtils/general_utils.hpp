//
//  general_utils.h
//  TestIphoneCV
//
//  Created by Dat Nguyen Thanh on 12/9/17.
//  Copyright © 2018 Dat Nguyen Thanh. All rights reserved.
//

#ifndef general_utils_h
#define general_utils_h

#include <vector>
#include <algorithm>
#include <numeric>
#include <stdexcept>

namespace DatCustom{
    namespace Util{
        template <typename T> std::vector<size_t> sortIndex(const std::vector<T>& v, bool inverse){
            std::vector<size_t> idx(v.size());
            std::iota(idx.begin(), idx.end(), 0);
            
            if (!inverse) std::sort(idx.begin(), idx.end(), [&v](size_t i1, size_t i2) {return v[i1] < v[i2];});
            else std::sort(idx.begin(), idx.end(), [&v](size_t i1, size_t i2) {return v[i1] > v[i2];});
            
            return idx;
        }
    }
		namespace Exception{
			/* @brief use to throw errors whenever user tries to use and unimplemendted function
			 */
			class NotImplementedException: public std::logic_error{
				public:
					NotImplementedException(): std::logic_error("Function not yet implemented"){};
			};
		}
}
#endif /* general_utils_h */
