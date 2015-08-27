// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#ifndef __L10N_H__
#define __L10N_H__

#include "asterix.h"

/**get the localized string by the key, if can't get the value then return mComment
 support for file name below:
 "Localized_en";//english
 "Localized_zh";//chinese
 "Localized_fr";//french
 "Localized_it";//italian
 "Localized_ge";//german
 "Localized_sp";//spanish
 "Localized_ru";//russian
 "Localized_ko";//korean
 "Localized_jp";//japanese
 "Localized_hu";//hungarian
 */
PCTSTR GetL10NStr(PCTSTR key, PCTSTR dft);

/**purge localize string cached
 */
void PurgeL10NCached();





#endif
