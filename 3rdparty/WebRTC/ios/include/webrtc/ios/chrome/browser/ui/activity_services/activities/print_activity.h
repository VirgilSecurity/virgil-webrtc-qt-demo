// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IOS_CHROME_BROWSER_UI_ACTIVITY_SERVICES_ACTIVITIES_PRINT_ACTIVITY_H_
#define IOS_CHROME_BROWSER_UI_ACTIVITY_SERVICES_ACTIVITIES_PRINT_ACTIVITY_H_

#import <UIKit/UIKit.h>

@protocol BrowserCommands;
@class ShareToData;

// Activity that triggers the printing service.
@interface PrintActivity : UIActivity

// Initializes the print activity with the given |data| and the |handler|.
- (instancetype)initWithData:(ShareToData*)data
                     handler:(id<BrowserCommands>)handler
    NS_DESIGNATED_INITIALIZER;
- (instancetype)init NS_UNAVAILABLE;

@end

#endif  // IOS_CHROME_BROWSER_UI_ACTIVITY_SERVICES_ACTIVITIES_PRINT_ACTIVITY_H_
