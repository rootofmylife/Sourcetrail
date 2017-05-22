#include "utility/file/FileRegister.h"

#include "utility/file/FilePath.h"

FileRegister::FileRegister(
	const FileRegisterStateData& stateData,
	const FilePath& currentPath,
	const std::set<FilePath>& indexedPaths,
	const std::set<FilePath>& excludedPaths
)
	: m_stateData(stateData)
	, m_currentPath(currentPath)
	, m_indexedPaths(indexedPaths)
	, m_excludedPaths(excludedPaths)
	, m_hasFilePathCache(
		[&](std::string f){
			const FilePath filePath(f);
			bool ret = false;

			if (filePath == m_currentPath)
			{
				ret = true;
			}

			if (!ret)
			{
				for (const FilePath& indexedPath: m_indexedPaths)
				{
					if (indexedPath.contains(filePath))
					{
						ret = true;
						break;
					}
				}
			}

			if (ret)
			{
				for (const FilePath& excluded: m_excludedPaths)
				{
					if (excluded.isDirectory())
					{
						if (excluded.contains(filePath))
						{
							ret = false;
							break;
						}
					}
					else
					{
						if (excluded == filePath)
						{
							ret = false;
							break;
						}
					}
				}
			}
			return ret;
		}
	)
{
}


FileRegister::~FileRegister()
{
}

const FileRegisterStateData& FileRegister::getStateData() const
{
	return m_stateData;
}

void FileRegister::markFileIndexing(const FilePath& filePath)
{
	m_stateData.markFileIndexing(filePath);
}

void FileRegister::markIndexingFilesIndexed()
{
	m_stateData.markIndexingFilesIndexed();
}

bool FileRegister::fileIsIndexed(const FilePath& filePath) const
{
	return m_stateData.fileIsIndexed(filePath);
}

bool FileRegister::hasFilePath(const FilePath& filePath) const
{
	return m_hasFilePathCache.getValue(filePath.str());
}
