// Copyright 2018 The Bazel Authors. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

use std::env::args;
use std::error;
use std::fs::File;
use std::path::Path;
use std::process::Command;

fn ensure() -> Result<(),Box<dyn error::Error>> {
	let index = args().position(|a| a=="--").ok_or("no --")?;
	let optional_outputs = args().take(index).collect::<Vec<String>>();
	let exe = args().nth(index+1).ok_or("no exe")?;
	let exe_args = args().skip(index+2).collect::<Vec<String>>();
	if exe_args.len() < 1 {
		return Err("no exe args".into());
	}
	let _ = Command::new(exe).args(exe_args).output();
	for out in optional_outputs {
		if !Path::new(&out).exists() {
			let _ = File::create(out)?;
		}
	}
	Ok(())
}

fn main() {
	if let Err(e) = ensure() {
		println!("Usage: [optional_output1...optional_outputN] -- program [arg1...argn]");
		println!("{:?}",args());
		println!("{:?}",e);
	}
}
