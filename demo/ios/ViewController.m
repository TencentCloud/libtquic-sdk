//
//  ViewController.m
//  Tquic
//
//  Created by "jourhuang" on 02/23/2021.
//  Copyright (c) 2021 "jourhuang". All rights reserved.
//

#import "ViewController.h"

#import <TquicPod/TquicPod.h>

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    

    TQUICRequest *request = [TQUICRequest requestWithBlock:^(TQUICRequest * _Nonnull request) {
        [request addUrl:@"https://www.example.com"];
        [request addPort:443];
    }];
    
    TQUICConfig *config = TQUICConfig.new;
    [config addTotalTimeoutMS:800];
    [config addUploadOptimize:YES];
    [config addCongestionType:TQUICCongestionTypeBBR];
    [config addRaceType:TQUICRaceTypeOnlyQUIC];
    [config addQuicVersion:43];

    [TQUICService startRequest:request completion:^(NSData * _Nonnull data, TQUICStats * _Nonnull stats, NSError * _Nonnull error) {
        NSLog(@"data: %@, error: %@", data, error);
    }];
}




@end
