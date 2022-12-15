#!/usr/bin/env bash

source ./common_header.sh

originalListSpaceName=SkimmedFiles_2017Background_Lists
originalListSpace="/afs/cern.ch/work/y/yuehshun/private/Projects/fasyakhuza/MonoZtoLL/optimization/${originalListSpaceName}"
# originalListSpace=./ntuple_filelist
outputSpace=/eos/user/y/yuehshun/${originalListSpaceName}_output.tmp
outputMergedSpace=/eos/user/y/yuehshun/${originalListSpaceName}_outputmerged.tmp

subDirRoot="";
dryRun=0
force=0

while (( "$#" )); do
	case "$1" in
		-h|--help)
			cat <<END_OF_HELP
END_OF_HELP
			exit 0
			;;
		--dry-run)
			dryRun=1
			shift
			;;
		-f|--force)
			force=1
			shift
			;;
		--maxdepth)
			maxDepth="$2"
			shift 2
			;;
		--mindepth)
			minDepth="$2"
			shift 2
			;;
		--original-list-space)
			originalListSpace="$2"
			shift 2
			;;
		--output-merged-space)
			outputMergedSpace="$2"
			shift 2
			;;
		--output-space)
			outputSpace="$2"
			shift 2
			;;
		--subdir)
			subDirRoot="$2"
			shift 2
			;;
		--?*)
			echo "Unexpected flag $1"
			exit 1
			;;
		--)
			echo "Unexpected flag $1"
			exit 1
			;;
		-??*)
			manageShorthands
			;;
		-?)
			echo "Unexpected flag $1"
			exit 1
			;;
		*)
			echo "Unexpected argumnet $1" >&2
			exit 1
			;;
	esac
done

# if [[ -z "$subDirRoot" ]]; then
# 	echo "subDirRoot is usually not set as empty." >&2
# 	echo "Set it with --subdir subDirRoot" >&2
# fi

declare -a forceArgArray=()
if (( force )); then
	forceArgArray=( "-f" )
fi

declare -a searchCommand=()
# If subDirRoot is empty, it will find "$originalListSpace"
searchCommand+=( find "$originalListSpace${subDirRoot:+/$subDirRoot}" )
if [[ -n "${minDepth:-}" ]]; then
	searchCommand+=( "-mindepth" "$minDepth" )
fi
if [[ -n "${maxDepth:-}" ]]; then
	searchCommand+=( "-maxdepth" "$maxDepth" )
fi
searchCommand+=( "-name" "*.txt" )
searchCommand+=( "-print0" )

while IFS= read -r -d $'\0' pathOriginalList; do
	# Change the prefix and remove the .txt file extension
	subDirPlusPrefSlash="${pathOriginalList:${#originalListSpace}:-4}"
	declare -a filesToMerge=()
	if [[ ! -d "$outputSpace$subDirPlusPrefSlash" && ! ( -L "$outputSpace$subDirPlusPrefSlash" && -d "$(realpath "$outputSpace$subDirPlusPrefSlash")" ) ]]; then
		echo "$outputSpace$subDirPlusPrefSlash is not a directory / a symlink to a directory" >&2
		# exit 1
	fi
	filesToMerge=()
	while IFS= read -r -d $'\0' file; do
		filesToMerge+=( "$file" )
	done < <(find "$outputSpace$subDirPlusPrefSlash" -mindepth 1 -maxdepth 1 -name "*.root" -print0)
	wrapDryRun mkdir -p "$(dirname "${outputMergedSpace}${subDirPlusPrefSlash}_merged.root")"
	wrapDryRun hadd "${forceArgArray[@]}" "${outputMergedSpace}${subDirPlusPrefSlash}_merged.root" "${filesToMerge[@]}"
done < <("${searchCommand[@]}")
