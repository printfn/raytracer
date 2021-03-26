#pragma once

#ifndef SCENE_READER_H_INCLUDED
#define SCENE_READER_H_INCLUDED

#include "NonCopyable.h"

#include "Colour.h"
#include "Material.h"
#include "Scene.h"

#include <map>
#include <queue>
#include <string>

/// Simple parser for Scene description files.
class SceneReader : private NonCopyable {

public:
    /** \brief SceneReader constructor.
     */
    explicit SceneReader(const std::string& filename);

    Scene &getScene();

private:

    /** \brief Read Scene data from a file.
     *
     * This method reads data from a file in the format discussed above.
     * It adds information to the Scene linked to this SceneReader, and so
     * multiple files can be combined into one Scene.
     *
     * If an error is encountered parsing the file, the program is terminated.
     *
     * \param filename The name of the file to read.
     */
    void read(const std::string &filename);

    /** \brief Parse a block of tokens. 
     *
     * When reading a file, it is separated into a squence of tokens (words and numbers)
     * separated by whitespace. These are formed into blocks, each ending with the token 'End'.
     * This method determines how to interpret each block.
     *
     * \param tokenBlock A sequence of tokens to be interpreted.
     */
    void parseTokenBlock(std::queue<std::string> &tokenBlock);

    /** \brief Read Colour information from a block of tokens.
     *
     * This tries to interpret the next three items in a block of tokens as
     * a Colour. Three tokens are taken from the block and interpreted as 
     * red, green, and blue values for the Colour. 
     * 
     * If there is a problem in this process, the program is terminated.
     *
     * \param tokenBlock A sequence of tokens to read the Colour from.
     * \return The Colour read from the block of tokens..
     */
    Colour parseColour(std::queue<std::string> &tokenBlock);

    /** \brief Read a number information from a block of tokens.
     *
     * This tries to interpret the next item in a block of tokens as
     * a number. The token is removed from the block.
     * 
     * If there is a problem in this process, the program is terminated.
     *
     * \param tokenBlock A sequence of tokens to read the Colour from.
     * \return The Colour read from the block of tokens..
     */
    double parseNumber(std::queue<std::string> &tokenBlock) const;

    /** \brief Parse a block of tokens representing a Scene. 
     *
     * This method reads Scene information from a block of tokens.
     * The format for Scene blocks is described above, and any errors 
     * in parsing the block will terminate the program.
     *
     * \param tokenBlock A sequence of tokens to be interpreted.
     */
    void parseSceneBlock(std::queue<std::string> &tokenBlock);

    /** \brief Parse a block of tokens representing a Scene. 
     *
     * This method reads Scene information from a block of tokens.
     * The format for Scene blocks is described above, and any errors 
     * in parsing the block will terminate the program.
     *
     * \param tokenBlock A sequence of tokens to be interpreted.
     */
    void parseCameraBlock(std::queue<std::string> &tokenBlock);

    /** \brief Parse a block of tokens representing a Camera. 
     *
     * This method reads Camera information from a block of tokens.
     * The format for Camera blocks is described above, and any errors 
     * in parsing the block will terminate the program.
     *
     * \param tokenBlock A sequence of tokens to be interpreted.
     */
    void parseLightBlock(std::queue<std::string> &tokenBlock);

    /** \brief Parse a block of tokens representing a LightSource. 
     *
     * This method reads LightSource information from a block of tokens.
     * The format for LightSource blocks is described above, and any errors 
     * in parsing the block will terminate the program.
     *
     * \param tokenBlock A sequence of tokens to be interpreted.
     */
    void parseObjectBlock(std::queue<std::string> &tokenBlock);

    /** \brief Parse a block of tokens representing an Object. 
     *
     * This method reads Object information from a block of tokens.
     * The format for Object blocks is described above, and any errors 
     * in parsing the block will terminate the program.
     *
     * \param tokenBlock A sequence of tokens to be interpreted.
     */
    void parseMaterialBlock(std::queue<std::string> &tokenBlock);

    std::unique_ptr<Scene> scene_; //!< The Scene which information is read to.
    int startLine_; //!< The first line of the current block being parsed, for error reporting.
    std::map<std::string, Material> materials_; //!< A dictionary of Material types that have been read, and which can be used for subsequent Object properties.
};

#endif
