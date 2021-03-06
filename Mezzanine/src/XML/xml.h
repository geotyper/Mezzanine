// © Copyright 2010 - 2017 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

	The Mezzanine Engine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	The Mezzanine Engine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
	- DRM or Copy Protection of any kind(except Copyrights)
	- Software Patents You Do Not Wish to Freely License
	- Any Kind of Linking to Non-GPL licensed Works
	- Are Currently In Violation of Another Copyright Holder's GPL License
	- If You want to change our code and not add a few hundred MB of stuff to
		your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
/*
 * pugixml parser - version 1.2
 * --------------------------------------------------------
 * Copyright © 2006-2012, by Arseny Kapoulkine (arseny.kapoulkine@gmail.com)
 * Report bugs and download new versions at http://pugixml.org/
 *
 * This library is distributed under the MIT License. See notice at the end
 * of this file.
 *
 * This work is based on the pugxml parser, which is:
 * Copyright © 2003, by Kristen Wegner (kristen@tima.net)
 */
#ifndef _XML_H
#define _XML_H

#ifndef XML_VERSION
// Define version macro; evaluates to major * 100 + minor so that it's safe to use in less-than comparisons
/// @def XML_VERSION This correlates to the version of pugixml this uses
/// @brief
#	define XML_VERSION 100
#endif

#include "swig.h"
#ifdef SWIG_XML
    #ifdef SWIG_UNSAFE
        %module MezzanineXML
    #else
        #define SWIG_SAFE
        %module MezzanineXMLSafe
    #endif
    #define SWIG_MODULE_SET
#endif

#include "XML/objectrange.h"
#include "XML/attribute.h"
#include "XML/attributeiterator.h"
#include "XML/document.h"
#include "XML/memorymanagement.h"
#include "XML/nodeiterator.h"
#include "XML/nodetext.h"
#include "XML/node.h"
#include "XML/parseresult.h"
#include "XML/treewalker.h"
#include "XML/writer.h"
#include "XML/xmlenumerations.h"
#include "XML/xmlstring.h"
#include "XML/xpathnode.h"
#include "XML/xpathnodeset.h"
#include "XML/xpathparseresult.h"
#include "XML/xpathvariable.h"
#include "XML/xpathvariableset.h"
#include "XML/xpathquery.h"


namespace Mezzanine
{
    /// @brief All tools for working with XML are located in this namespace
    /// @details The contents of this namespace has many details covered  in the @ref XMLManual
    namespace XML
    {}
}

/// @page XMLManual Mezzanine::xml Manual
/// This will describe how to use the XML components of the Mezzanine
/// @section XMLTOC Table Of Contents
///     - @ref XMLOverview
///         - @ref XMLIntroduction
///         - @ref XMLFeedBack
///         - @ref XMLAcknowledgments
///         - @ref XMLLicense
///     - @ref XMLDOM
///         - @ref XMLTreeStructure
///         - @ref XMLInterface
///         - @ref XMLThreadSafety
///         - @ref XMLExceptionSafety
///         - @ref XMLMemory
///             - @ref XMLCustomAlloc
///             - @ref XMLMemoryInternals
///     - @ref XMLLoading
///         - @ref XMLLoadingFiles
///         - @ref XMLLoadingFromMemory
///         - @ref XMLLoadingStreams
///         - @ref XMLLoadingErrors
///         - @ref XMLLoadingParsingOptions
///         - @ref XMLLoadingEncodings
///         - @ref XMLLoadingStandards
///     - @ref XMLAccessing
///         - @ref XMLAccessingBasics
///         - @ref XMLAccessingNodeData
///         - @ref XMLAccessingAttributeData
///         - @ref XMLAccessingContentBased
///         - @ref XMLAccessingIterators
///         - @ref XMLAccessingWalker
///         - @ref XMLAccessingPredicates
///         - @ref XMLAccessingMisc
///     - @ref XMLModifying
///         - @ref XMLModifyingNodeData
///         - @ref XMLModifyingAttributeData
///         - @ref XMLModifyingAdding
///         - @ref XMLModifyingRemoving
///         - @ref XMLModifyingCloning
///     - @ref XMLSaving
///         - @ref XMLSavingFile
///         - @ref XMLSavingStreams
///         - @ref XMLSavingWriter
///         - @ref XMLSavingSubtree
///         - @ref XMLSavingOptions
///         - @ref XMLSavingEncodings
///     - @ref XMLXPath
///         - @ref XMLXPathTypes
///         - @ref XMLXPathSelecting
///         - @ref XMLXPathQuery
///         - @ref XMLXPathVariables
///         - @ref XMLXPathError
///         - @ref XMLXPathStandards
///
/// \n \n
/// @section XMLOverview Overview
///     - @ref XMLIntroduction
///     - @ref XMLFeedBack
///     - @ref XMLAcknowledgments
///     - @ref XMLLicense
///
/// @subsection XMLIntroduction Introduction
/// Mezzanine::xml is a light-weight C++ XML processing library. It consists of a DOM-like interface with rich traversal/modification capabilities, an
/// extremely fast XML parser which constructs the DOM tree from an XML file/buffer, and an @ref Mezzanine::XML::XPathQuery "XPath 1.0 implementation"
/// for complex data-driven tree queries. Unicode is fully support via UF8 in 8bit characters and for
/// conversions between different Unicode encodings (which happen automatically during parsing/saving).
/// \n \n
/// Mezzanine::xml enables very fast, convenient and memory-efficient XML document processing. However, since Mezzanine::xml has a DOM parser, it can't process
/// XML documents that do not fit in memory; also the parser is a non-validating one, so if you need DTD or XML Schema validation, the XML parser
/// is not for you.
/// \n \n
/// @subsection XMLFeedBack FeedBack
/// If you believe you've found a bug in Mezzanine::xml (bugs include compilation problems (errors/warnings), crashes, performance degradation and incorrect
/// behavior), please contact Blacktopp Studios Inc ( http://www.blacktoppstudios.com/ ) . We check the the Forums ( http://www.blacktoppstudios.com/?page_id=753 )
/// and items sent by our contact form ( http://www.blacktoppstudios.com/?page_id=33 ) regularly. Be sure to include the relevant information so that
/// the bug can be reproduced: the version of Mezzanine::xml, compiler version and target architecture, the code that uses Mezzanine::xml and exhibits the bug, etc.
/// \n \n
/// Feature requests can be reported the same way as bugs, so if you're missing some functionality in Mezzanine::xml or if the API is rough in some places
/// and you can suggest an improvement, please let us know. However, please note that there are many factors when considering API changes (compatibility
/// with previous versions, API redundancy, etc.).
/// \n \n
/// If you have a contribution to Mezzanine::xml, such as build script for some build system/IDE, or a well-designed set of helper functions, or a binding
/// to some language other than C++, please let us know. You can include the relevant patches as issue attachments. We will have to communicate on the
/// Licensing terms of your contribution though.
/// \n \n
/// If the provided methods of contact have an issue or not possible due to privacy or other concerns, you can contact the Mezzanine::xml author ( toppij@blacktoppstudios.com )
/// or pugixml author ( arseny.kapoulkine@gmail.com ) by e-mail directly. If you have an issue that pertains to pugixml and not Mezzanine::xml you can visit the
/// pugixml issue submission form ( http://code.google.com/p/pugixml/issues/entry ) of the pugixml feature request form
/// ( http://code.google.com/p/pugixml/issues/entry?template=Feature%20request ).
/// \n \n
/// @subsection XMLAcknowledgments Acknowledgments
/// Mezzanine::xml and pugixml could not be developed without the help from many people; some of them are listed in this section. If you've played a part in
/// Mezzanine::xml or pugixml development and you can not find yourself on this list, I'm truly sorry; please send me an e-mail ( toppij@blacktoppstudios.com )
/// so I can fix this.
/// \n \n
/// Thanks to \b Arseny \b Kapoulkine for pugixml parser, which was used as a basis for Mezzanine::xml.
/// \n \n
/// Thanks to \b Kristen \b Wegner for pugxml parser, which was used as a basis for pugixml.
/// \n \n
/// Thanks to \b Neville \b Franks for contributions to pugxml parser.
/// \n \n
/// Thanks to \b Artyom \b Palvelev for suggesting a lazy gap contraction approach.
/// \n \n
/// Thanks to \b Vyacheslav \b Egorov for documentation proofreading.
/// \n \n
/// @subsection XMLLicense License
/// With written permission as per @ref OriginalpugixmlLicense "The original pugixml license" we he sublicensed Mezzanine::xml under the @ref GPLLicense "GPL Version 3". In short
/// This allows you to use Mezzanine::xml however you like with a few restrictions. If you change Mezzanine::xml you need to make the changes publically available.
/// If you make software using Mezzanine::xml you need to make the source code publicly available. You may not use and Digital Rights Management (DRM) software
/// to limit how others use the combined work you make. You can sell resulting works, but not through a digital distribution store that uses DRM.
///
/// \n \n
/// @section XMLDOM Document Object Model
/// Mezzanine::xml stores XML data in DOM-like way: the entire XML document (both document structure and element data) is stored in memory as a tree. The tree can be
/// loaded from a character stream (file, string, C++ I/O stream), then traversed with the special API or XPath expressions. The whole tree is mutable: both node
/// structure and node/attribute data can be changed at any time. Finally, the result of document transformations can be saved to a character stream (file, C++
/// I/O stream or custom transport).
///     - @ref XMLTreeStructure
///     - @ref XMLInterface
///     - @ref XMLThreadSafety
///     - @ref XMLExceptionSafety
///     - @ref XMLMemory
///         - @ref XMLCustomAlloc
///         - @ref XMLMemoryInternals
///
/// @subsection XMLTreeStructure Tree structure
/// The XML document is represented with a tree data structure. The root of the tree is the document itself, which corresponds to C++ type Mezzanine::XML::Document.
/// A Document has one or more child nodes, which correspond to C++ type Mezzanine::XML::Node. Nodes have different types; depending on a type, a node can have a
/// collection of child nodes, a collection of attributes, which correspond to C++ type Mezzanine::XML::Attribute, and some additional data (i.e. Name).
/// \n \n
/// The tree nodes can be of one of the following types (which together form the enumeration Mezzanine::XML::NodeType):
///     - @ref Mezzanine::XML::NodeType "NodeType::NodeDocument Document node" - This is the root of the tree, which consists of several child nodes. This node corresponds
///     to Mezzanine::XML::Document class; note that Mezzanine::XML::Document is a sub-class of Mezzanine::XML::Node, so the entire node interface is also available. However,
///     document nodes are special in several ways, which are covered below. There can be only one document node in the tree; document node does not have any XML
///     representation. \n
///     - @ref Mezzanine::XML::NodeType "NodeType::NodeElement Element/tag node" - This is the most common type of node, which represents XML elements. Element nodes
///     have a name, a collection of attributes and a collection of child nodes (both of which may be empty). The attribute is a simple name/value pair. The example
///     XML representation of element nodes is as follows:
///     @code{.xml}
///         <node attr="value"><child/></node>
///     @endcode
///     There are two element nodes here: one has name "node", single attribute "attr" and the single child "child" which has the name "child" and does not have
///     any attributes or child nodes. \n
///     - @ref Mezzanine::XML::NodeType "NodeType::NodePcdata Plain character data node" - Represent plain text in XML. PCDATA nodes have a value, but do not have a name
///     or children/attributes. Note that plain character data is not a part of the element node but instead has its own node; for example, an element node can have
///     several child PCDATA nodes. The example XML representation of text nodes is as follows:
///     @code
///         <node> text1 <child/> text2 </node>
///     @endcode
///     Here "node" element has three children, two of which are PCDATA nodes with values "text1" and "text2". \n
///     - @ref Mezzanine::XML::NodeType "NodeType::NodeCdata Character data nodes" - These represent text in XML that is quoted in a special way. CDATA nodes do not differ
///     from PCDATA nodes except in XML representation - the above text example looks like this with CDATA:
///     @code
///         <node> <![CDATA[[text1]]> <child/> <![CDATA[[text2]]> </node>
///     @endcode
///     CDATA nodes make it easy to include non-escaped \<, \& and \> characters in plain text. CDATA value can not contain the character sequence ]]\>, since it is
///     used to determine the end of node contents. \n
///     - @ref Mezzanine::XML::NodeType "NodeType::NodeComment Comment nodes" - represent comments in XML. Comment nodes have a value, but do not have a name or
///     children/attributes. The example XML representation of a comment node is as follows:
///     @code
///         <!-- comment text -->
///     @endcode
///     Here the comment node has value "comment text". By default comment nodes are treated as non-essential part of XML markup and are not loaded during XML parsing.
///     You can override this behavior with Mezzanine::XML::ParseComments flag. \n
///     - @ref Mezzanine::XML::NodeType "NodeType::NodePi Processing instruction node" - Represent Processing Instructions (PI) in XML. PI nodes have a name and an optional
///     value, but do not have children/attributes. The example XML representation of a PI node is as follows:
///     @code
///         <?name value?>
///     @endcode
///     Here the name (also called PI target) is "name", and the value is "value". By default PI nodes are treated as non-essential part of XML markup and are not
///     loaded during XML parsing. You can override this behavior with Mezzanine::XML::ParsePi flag. \n
///     - @ref Mezzanine::XML::NodeType "NodeType::NodeDeclaration Declaration node" - Represents document declarations in XML. Declaration nodes have a name ("xml") and an
///     optional collection of attributes, but do not have value or children. There can be only one declaration node in a document; moreover, it should be the topmost
///     node (its parent should be the document). The example XML representation of a declaration node is as follows:
///     @code
///         <?xml version="1.0"?>
///     @endcode
///     Here the node has name "xml" and a single attribute with name "version" and value "1.0". By default declaration nodes are treated as non-essential part of XML
///     markup and are not loaded during XML parsing. You can override this behavior with Mezzanine::XML::ParseDeclaration flag. Also, by default a dummy declaration is
///     output when XML document is saved unless there is already a declaration in the document; you can disable this with Mezzanine::XML::FormatNoDeclaration flag. \n
///     - @ref Mezzanine::XML::NodeType "NodeType::NodeDocType Document type declaration node" - Represents document type declarations in XML. Document type declaration nodes
///     have a value, which corresponds to the entire document type contents; no additional nodes are created for inner elements like \<!ENTITY\>. There can be only one
///     document type declaration node in a document; moreover, it should be the topmost node (its parent should be the document). The example XML representation of a
///     document type declaration node is as follows:
///     @code
///         <!DOCTYPE greeting [ <!ELEMENT greeting (#PCDATA)> ]>
///     @endcode
///     Here the node has value "greeting [ <!ELEMENT greeting (#PCDATA)> ]". By default document type declaration nodes are treated as non-essential part of XML markup
///     and are not loaded during XML parsing. You can override this behavior with Mezzanine::XML::ParseDocType flag. \n
/// \n \n
/// Finally, here is a complete example of XML document and the corresponding tree representation:
/// @code
/// <?xml version="1.0"?>
/// <mesh name="mesh_root">
///     <!-- here is a mesh node -->
///     some text
///     <![CDATA[someothertext]]>
///     some more text
///     <node attr1="value1" attr2="value2" />
///     <node attr1="value2">
///         <innernode/>
///     </node>
/// </mesh>
/// <?include somedata?>
/// @endcode
/// @image html SampleTree.jpg "Complete Tree Representation of the Sample"
/// @image latex SampleTree.jpg "Complete Tree Representation of the Sample"
/// @image rtf SampleTree.jpg "Complete Tree Representation of the Sample"
/// @subsection XMLInterface C++ interface
/// Despite the fact that there are several node types, there are only three C++ classes representing the tree (Mezzanine::XML::Document, Mezzanine::XML::Node, Mezzanine::XML::Attribute);
/// some operations on Mezzanine::XML::Node are only valid for certain node types. The classes are described below.
/// \n \n
/// Mezzanine::XML::Document is the owner of the entire document structure; it is a non-copyable class. The interface of Mezzanine::XML::Document consists of loading functions
/// ( see @ref XMLLoading ), saving functions ( see @ref XMLSaving ) and the entire interface of Mezzanine::XML::Node, which allows for document inspection and/or modification.
/// Note that while Mezzanine::XML::Document is a sub-class of Mezzanine::XML::Node, Mezzanine::XML::Node is not a polymorphic type; the inheritance is present only to simplify usage.
/// Alternatively you can use the Mezzanine::XML::Document::DocumentElement function to get the element node that's the immediate child of the document.
/// \n \n
/// Default constructor of Mezzanine::XML::Document initializes the document to the tree with only a root node ( Mezzanine::XML::Document node). You can then populate it with data
/// using either tree modification functions or loading functions; all loading functions destroy the previous tree with all occupied memory, which puts existing
/// node/attribute handles for this document to invalid state. If you want to destroy the previous tree, you can use the Mezzanine::XML::Document::Reset function; it destroys
/// the tree and replaces it with either an empty one or a copy of the specified document. Destructor of Mezzanine::XML::Document also destroys the tree, thus the lifetime of
/// the document object should exceed the lifetimes of any node/attribute handles that point to the tree.
/// @warning While technically node/attribute handles can be alive when the tree they're referring to is destroyed, calling any member function for these handles results
/// in undefined behavior. Thus it is recommended to make sure that the document is destroyed only after all references to its nodes/attributes are destroyed.
///
/// Mezzanine::XML::Node is the handle to a document node; it can point to any node in the document, including the document node itself. There is a common interface for nodes
/// of all types; the actual node type can be queried via the Mezzanine::XML::Node::Type() method. Note that Mezzanine::XML::Node is only a handle to the actual node, not the node
/// itself - you can have several Mezzanine::XML::node handles pointing to the same underlying object. Destroying Mezzanine::XML::Node handle does not destroy the node and does not
/// remove it from the tree. The size of Mezzanine::XML::Node is equal to that of a pointer, so it is nothing more than a lightweight wrapper around a pointer; you can safely
/// pass or return Mezzanine::xml:Node objects by value without additional overhead.
/// \n \n
/// There is a special value of Mezzanine::XML::Node type, known as null node or empty node (such nodes have type NodeNull). It does not correspond to any node in any document,
/// and thus resembles null pointer. However, all operations are defined on empty nodes; generally the operations don't do anything and return empty nodes/attributes or
/// empty strings as their result (see documentation for specific functions for more detailed information). This is useful for chaining calls; i.e. you can get the
/// grandparent of a node like so: node.GetParent().GetParent(); if a node is a null node or it does not have a parent, the first Node::GetParent() call returns null node;
/// the second GetParent() call then also returns null node, which can make error handling easier.
/// \n \n
/// Mezzanine::XML::Attribute is a handle to an XML attribute; it has the same semantics as Mezzanine::XML::Node, i.e. there can be several Mezzanine::XML::Attribute handles pointing to
/// the same underlying object and there is a special null attribute value, which propagates to function results.
/// \n \n
/// Both Mezzanine::XML::Node and Mezzanine::XML::Attribute have the default constructor which initializes them to null objects.
/// \n \n
/// Mezzanine::XML::Node and Mezzanine::XML::Attribute try to behave like pointers, that is, they can be compared with other objects of the same type, making it possible to use them
/// as keys in associative containers. All handles to the same underlying object are equal, and any two handles to different underlying objects are not equal. Null handles
/// only compare as equal to themselves. The result of relational comparison can not be reliably determined from the order of nodes in file or in any other way. Do not use
/// relational comparison operators except for search optimization (i.e. associative container keys).
/// \n \n
/// If you want to use Mezzanine::XML::Node or Mezzanine::XML::Attribute objects as keys in hash-based associative containers, you can use the Mezzanine::XML::Node::HashValue or
/// Mezzanine::XML::Attribute::HashValue member functions. They return the hash values that are guaranteed to be the same for all handles to the same underlying object. The hash
/// value for null handles is 0.
/// \n \n
/// Finally handles can be implicitly cast to boolean-like objects, so that you can test if the node/attribute is empty with the following code: if (node) { ... } or if
/// (!node) { ... } else { ... }. Alternatively you can check if a given Node/Attribute handle is null by calling the Mezzanine::XML::Attribute::Empty or the
/// Mezzanine::XML::Node::Empty Methods.
/// \n \n
/// Nodes and attributes do not exist without a document tree, so you can't create them without adding them to some document. Once underlying node/attribute objects are
/// destroyed, the handles to those objects become invalid. While this means that destruction of the entire tree invalidates all node/attribute handles, it also means that
/// destroying a subtree ( by calling Mezzanine::XML::Node::RemoveChild ) or removing an attribute invalidates the corresponding handles. There is no way to check handle
/// validity; you have to ensure correctness through external mechanisms.
/// \n \n
/// @subsection XMLThreadSafety Thread-safety guarantees
///Almost all functions in Mezzanine::xml have the following thread-safety guarantees:
///     - It is safe to call free (non-member) functions from multiple threads
///     - It is safe to perform concurrent read-only accesses to the same tree (all constant member functions do not modify the tree)
///     - It is safe to perform concurrent read/write accesses, if there is only one read or write access to the single tree at a time
///
/// Concurrent modification and traversing of a single tree requires synchronization, for example via reader-writer lock. Modification includes altering document structure
/// and altering individual node/attribute data, i.e. changing names/values.
/// \n \n
/// The only exception is Mezzanine::XML::SetMemory_management_functions; it modifies global variables and as such is not thread-safe. Its usage policy has more restrictions, see
/// @ref XMLCustomAlloc .
/// \n \n
/// @subsection XMLExceptionSafety Exception guarantees
/// With the exception of XPath, Mezzanine::xml itself does not throw any exceptions. Additionally, most Mezzanine::xml functions have a no-throw exception guarantee.
/// \n \n
/// This is not applicable to functions that operate on STL strings or IOstreams; such functions have either strong guarantee (functions that operate on strings) or basic
/// guarantee (functions that operate on streams). Also functions that call user-defined callbacks (i.e. Mezzanine::XML::Node::Traverse or Mezzanine::XML::Node::FindNode) do not
/// provide any exception guarantees beyond the ones provided by the callback.
/// \n \n
/// XPath functions may throw Mezzanine::XML::XPathException on parsing errors; also, XPath functions may
/// throw std::bad_alloc in low memory conditions. Still, XPath functions provide strong exception guarantee.
/// \n \n
/// @subsection XMLMemory Memory management
/// Mezzanine::xml requests the memory needed for document storage in big chunks, and allocates document data inside those chunks. This section discusses replacing functions
/// used for chunk allocation and internal memory management implementation.
/// \n \n
/// @warning The Mezzanine::xml memory system is pending tighter integration with memory management from the rest of the system. The functions and features described here should be
/// considered deprecated and internal to the engine. It is highly likely they will be replaced without warning by a global system to allocate memory in a more efficient way.
///
/// \n \n
/// @subsubsection XMLCustomAlloc Custom memory allocation/deallocation functions
/// All memory for tree structure, tree data and XPath objects is allocated via globally specified functions, which default to malloc/free. You can set your own allocation
/// functions with Mezzanine::XML::SetMemory_management_functions function. The function interfaces are the same as that of malloc/free:
/// @code
/// typedef void* (*AllocationFunction)(size_t size);
/// typedef void (*deAllocationFunction)(void* ptr);
/// @endcode
/// You can use the following accessor functions to change or get current memory management functions:
/// @code
/// void Mezzanine::XML::SetMemory_management_functions(AllocationFunction allocate, deAllocationFunction deallocate);
/// AllocationFunction GetMemoryAllocationFunctionn();
/// deAllocationFunction GetMemoryDeallocationFunction();
/// @endcode
/// Allocation function is called with the size (in bytes) as an argument and should return a pointer to a memory block with alignment that is suitable for storage of primitive
/// types (usually a maximum of void* and double types alignment is sufficient) and size that is greater than or equal to the requested one. If the allocation fails, the function
/// has to return null pointer (throwing an exception from allocation function results in undefined behavior).
/// \n \n
/// Deallocation function is called with the pointer that was returned by some call to allocation function; it is never called with a null pointer. If memory management functions
/// are not thread-safe, library thread safety is not guaranteed.
/// \n \n
/// When setting new memory management functions, care must be taken to make sure that there are no live Mezzanine::xml objects. Otherwise when the objects are destroyed, the new
/// deallocation function will be called with the memory obtained by the old allocation function, resulting in undefined behavior.
/// \n \n
/// @subsubsection XMLMemoryInternals Document memory management internals
/// Constructing a document object using the default constructor does not result in any allocations; document node is stored inside the Mezzanine::XML::Document object
/// \n \n
/// When the document is loaded from file/buffer, unless an inplace loading function is used ( see @ref XMLLoadingFromMemory ), a complete copy of character stream is made; all
/// names/values of nodes and attributes are allocated in this buffer. This buffer is allocated via a single large allocation and is only freed when document memory is reclaimed
/// (i.e. if the Mezzanine::XML::Document object is destroyed or if another document is loaded in the same object). Also when loading from file or stream, an additional large allocation
/// may be performed if encoding conversion is required; a temporary buffer is allocated, and it is freed before load function returns.
/// \n \n
/// All additional memory, such as memory for document structure (node/attribute objects) and memory for node/attribute names/values is allocated in pages on the order of 32
/// kilobytes; actual objects are allocated inside the pages using a memory management scheme optimized for fast allocation/deallocation of many small objects. Because of the
/// scheme specifics, the pages are only destroyed if all objects inside them are destroyed; also, generally destroying an object does not mean that subsequent object creation
/// will reuse the same memory. This means that it is possible to devise a usage scheme which will lead to higher memory usage than expected; one example is adding a lot of nodes,
/// and them removing all even numbered ones; not a single page is reclaimed in the process. However this is an example specifically crafted to produce unsatisfying behavior;
/// in all practical usage scenarios the memory consumption is less than that of a general-purpose allocator because allocation meta-data is very small in size.
/// \n \n
/// @section XMLLoading Loading Documents
///     - @ref XMLLoadingFiles
///     - @ref XMLLoadingFromMemory
///     - @ref XMLLoadingStreams
///     - @ref XMLLoadingErrors
///     - @ref XMLLoadingParsingOptions
///     - @ref XMLLoadingEncodings
///     - @ref XMLLoadingStandards
///
/// Mezzanine::xml provides several functions for loading XML data from various places - files, C++ iostreams, memory buffers. All functions use an extremely fast non-validating parser.
/// This parser is not fully W3C conformant - it can load any valid XML document, but does not perform some well-formedness checks. While considerable effort is made to reject
/// invalid XML documents, some validation is not performed for performance reasons. Also some XML transformations (i.e. EOL handling or attribute value normalization) can impact
/// parsing speed and thus can be disabled. However for vast majority of XML documents there is no performance difference between different parsing options. Parsing options also
/// control whether certain XML nodes are parsed; see @ref XMLLoadingParsingOptions for more information.
/// \n \n
/// XML data is always converted to internal character format before parsing. Mezzanine::xml supports all popular Unicode encodings (UTF-8, UTF-16 (big and little
/// endian), UTF-32 (big and little endian); UCS-2 is naturally supported since it's a strict subset of UTF-16) and handles all encoding conversions automatically. Unless explicit
/// encoding is specified, loading functions perform automatic encoding detection based on first few characters of XML data, so in almost all cases you do not have to specify
/// document encoding. Encoding conversion is described in more detail in @ref XMLLoadingEncodings.
/// \n \n
/// @subsection XMLLoadingFiles Loading Document from a File
/// The most common source of XML data is files; Mezzanine::xml provides dedicated functions for loading an XML document from file:
/// @code
/// Mezzanine::XML::ParseResult Mezzanine::XML::Document::LoadFile(const char* path, unsigned int options = ParseDefault, Mezzanine::XML::Encoding DocumentEncoding = EncodingAuto);
/// Mezzanine::XML::ParseResult Mezzanine::XML::Document::LoadFile(const wchar_t* path, unsigned int options = ParseDefault, Mezzanine::XML::Encoding DocumentEncoding = EncodingAuto);
/// @endcode
/// These functions accept the file path as its first argument, and also two optional arguments, which specify parsing options (see Parsing options) and input data encoding ( see
/// @ref XMLLoadingEncodings ). The path has the target operating system format, so it can be a relative or absolute one, it should have the delimiters of the target system, it
/// should have the exact case if the target file system is case-sensitive, and is subject to any other restrictions of the underlying filesystem.
/// \n \n
/// File path is passed to the system file opening function as is in case of the first function (which accepts const char* path); the second function either uses a special file
/// opening function if it is provided by the runtime library or converts the path to UTF-8 and uses the system file opening function.
/// \n \n
/// Document::LoadFile destroys the existing document tree and then tries to load the new tree from the specified file. The result of the operation is returned in an
/// Mezzanine::XML::ParseResult object; this object contains the operation status and the related information (i.e. last successfully parsed position in the input file, if parsing
/// fails). See @ref XMLLoadingErrors for error handling details.
/// \n \n
/// This is an example of loading XML document from file:
/// @code
/// Mezzanine::XML::Document Doc;
/// Mezzanine::XML::ParseResult Result = Doc.LoadFile("tree.xml");
/// std::cout << "Load result: " << Result.Description() << std::endl;
/// @endcode
/// @subsection XMLLoadingFromMemory Loading document from memory
/// Sometimes XML data should be loaded from some other source than a file, i.e. HTTP URL; also you may want to load XML data from file using non-standard functions, i.e. to use
/// your virtual file system facilities or to load XML from gzip-compressed files. All these scenarios require loading document from memory. First you should prepare a contiguous
/// memory block with all XML data; then you have to invoke one of buffer loading functions. These functions will handle the necessary encoding conversions, if any, and then will
/// parse the data into the corresponding XML tree. There are several buffer loading functions, which differ in the behavior and thus in performance/memory usage:
/// @code
/// Mezzanine::XML::ParseResult Mezzanine::XML::Document::LoadBuffer (const void *contents, size_t size, unsigned int options=ParseDefault, Encoding DocumentEncoding=EncodingAuto);
/// Mezzanine::XML::ParseResult Mezzanine::XML::Document::LoadBufferInplace (void *contents, size_t size, unsigned int options=ParseDefault, Encoding DocumentEncoding=EncodingAuto);
/// Mezzanine::XML::ParseResult Mezzanine::XML::Document::LoadBufferInplaceOwn (void *contents, size_t size, unsigned int options=ParseDefault, Encoding DocumentEncoding=EncodingAuto)
/// @endcode
/// All functions accept the buffer which is represented by a pointer to XML data, contents, and data size in bytes. Also there are two optional arguments, which specify parsing
/// options ( see @ref XMLLoadingParsingOptions ) and input data encoding ( see @ref XMLLoadingEncodings ). The buffer does not have to be zero-terminated.
/// \n \n
/// Mezzanine::XML::Document::LoadBuffer function works with immutable buffer - it does not ever modify the buffer. Because of this restriction it has to create a private buffer and
/// copy XML data to it before parsing (applying encoding conversions if necessary). This copy operation carries a performance penalty, so inplace functions are provided -
/// Mezzanine::XML::Document::LoadBufferInplace and Mezzanine::XML::Document::LoadBufferInplaceOwn store the document data in the buffer, modifying it in the process. In order for the
/// document to stay valid, you have to make sure that the buffer's lifetime exceeds that of the tree if you're using inplace functions. In addition to that,
/// Mezzanine::XML::Document::LoadBufferInplace does not assume ownership of the buffer, so you'll have to destroy it yourself; Mezzanine::XML::Document::LoadBufferInplaceOwn assumes
/// ownership of the buffer and destroys it once it is not needed. This means that if you're using Mezzanine::XML::Document::LoadBufferInplaceOwn, you have to allocate memory with
/// Mezzanine::xml allocation function ( Not recomended, the Allocation API may be getting updates in the near future ).
/// \n \n
/// The best way from the performance/memory point of view is to load document using Mezzanine::XML::Document::LoadBufferInplaceOwn; this function has maximum control of the buffer
/// with XML data so it is able to avoid redundant copies and reduce peak memory usage while parsing. However, this is not recommendeded unless you have to load the document
/// from memory and performance is critical. Once the memory portion of the API has stabilized this will become the ideal
/// \n \n
/// There is also a simple helper function for cases when you want to load the XML document from null-terminated character string:
/// @code
/// Mezzanine::XML::ParseResult Mezzanine::XML::Document::Load(const char_t *contents, unsigned int options=ParseDefault);
/// @endcode
/// It is equivalent to calling Mezzanine::XML::Document::LoadBuffer with size being either strlen(contents) or wcslen(contents) * sizeof(wchar_t), depending on the character type.
/// This function assumes native encoding for input data, so it does not do any encoding conversion. In general, this function is fine for loading small documents from string
/// literals, but has more overhead and less functionality than the buffer loading functions.
/// \n \n
/// This is an example of loading XML document from memory using LoadBuffer:
/// @code
/// const char Source[] = "<mesh name='sphere'><bounds>0 0 1 1</bounds></mesh>";
/// size_t Size = sizeof(Source);
///
/// // You can use LoadBuffer to load the document from an immutable memory block:
/// Mezzanine::XML::ParseResult Result = doc.LoadBuffer(Source, Size);
/// @endcode
/// This is an example of loading XML document from memory using LoadBufferInplace:
/// @code
/// const char Source[] = "<mesh name='sphere'><bounds>0 0 1 1</bounds></mesh>";
/// size_t Size = sizeof(Source);
///
/// // You can use LoadBufferInplace to load document from mutable memory block
/// // the block's lifetime must exceed that of document
/// char* Buffer = new char[Size];
/// memcpy(Buffer, Source, Size);
///
/// // The block can be allocated by any method; the block is modified during parsing
/// Mezzanine::XML::ParseResult Result = doc.LoadBufferInplace(Buffer, Size);
///
/// // You have to destroy the block yourself after the document is no longer used
/// delete[] Buffer;
/// @endcode
/// This is an example of loading XML document from memory using Load and a string literal:
/// @code
/// // You can use Load to load document from null-terminated strings, for example literals:
/// Mezzanine::XML::ParseResult Result = doc.Load("<mesh name='sphere'><bounds>0 0 1 1</bounds></mesh>");
/// @endcode
/// @subsection XMLLoadingStreams Loading document from C++ IOstreams
/// To enhance interoperability, Mezzanine::xml provides functions for loading document from any object which implements C++ std::istream interface. This allows you to load documents
/// from any standard C++ stream (i.e. file stream) or any third-party compliant implementation (i.e. Boost Iostreams). There are two functions, one works with narrow character
/// streams, another handles wide character ones:
/// @code
/// Mezzanine::XML::ParseResult Mezzanine::XML::Document::Load (std::basic_istream< char, std::char_traits< char > > &stream, unsigned int options=ParseDefault, Encoding DocumentEncoding=EncodingAuto);
/// Mezzanine::XML::ParseResult Mezzanine::XML::Document::Load (std::basic_istream< wchar_t, std::char_traits< wchar_t > > &stream, unsigned int options=ParseDefault);
/// @endcode
/// Mezzanine::XML::Document::Load with std::istream argument loads the document from stream from the current read position to the end, treating the stream contents as a byte stream
/// of the specified encoding (with encoding autodetection as necessary). Thus calling Mezzanine::XML::Document::Load on an opened std::ifstream object is equivalent to calling
/// Mezzanine::XML::Document::LoadFile.
/// \n \n
/// Mezzanine::XML::Document::Load with std::wstream argument treats the stream contents as a wide character stream ( encoding is always @ref Mezzanine::XML::Encoding "Encoding::Encodingwchar_t" ).
/// Because of this, using Mezzanine::XML::Document::LoadFile with wide character streams requires careful (usually platform-specific) stream setup (i.e. using the imbue function).
/// Generally use of wide streams is discouraged, however it provides you the ability to load documents from non-Unicode encodings, i.e. you can load Shift-JIS encoded data if
/// you set the correct locale.
/// \n \n
/// This is a simple example of loading XML document from a file using streams read:
/// @code
/// std::ifstream Stream("weekly-utf-8.xml");
/// Mezzanine::XML::ParseResult Result = Doc.Load(Stream);
/// @endcode
/// Stream loading requires working seek/tell functions and therefore may fail when used with some stream implementations like gzstream.
/// \n \n
/// @subsection XMLLoadingErrors Handling Parsing Errors
/// All document loading functions return the parsing result via Mezzanine::XML::ParseResult object. It contains parsing status, the offset of last successfully parsed character from
/// the beginning of the source stream, and the encoding of the source stream.
/// \n \n
/// Parsing status is represented as the Mezzanine::XML::ParseStatus enumeration and can be one of the following:
///     - @ref Mezzanine::XML::ParseStatus "ParseStatus::StatusOk" means that no error was encountered during parsing; the source stream represents the valid XML document which was
///     fully parsed and converted to a tree.
///     - @ref Mezzanine::XML::ParseStatus "ParseStatus::StatusFileNotFound" is only returned by Document::LoadFile function and means that file could not be opened.
///     - @ref Mezzanine::XML::ParseStatus "ParseStatus::StatusIOError" is returned by Document::LoadFile function and by load functions with std::istream/std::wstream arguments; it
///     means that some I/O error has occurred during reading the file/stream.
///     - @ref Mezzanine::XML::ParseStatus "ParseStatus::StatusOutOfMemory" means that there was not enough memory during some allocation; any allocation failure during parsing results
///     in this error.
///     - @ref Mezzanine::XML::ParseStatus "ParseStatus::StatusInternalError" means that something went horribly wrong; currently this error does not occur.
///     - @ref Mezzanine::XML::ParseStatus "ParseStatus::StatusUnrecognizedTag" means that parsing stopped due to a tag with either an empty name or a name which starts with incorrect
///     character, such as #.
///     - @ref Mezzanine::XML::ParseStatus "ParseStatus::StatusBadPi" means that parsing stopped due to incorrect document declaration/processing instruction.
///     - @ref Mezzanine::XML::ParseStatus "ParseStatus::StatusBadComment, StatusBadCdata, StatusBadDocType and StatusBadPcdata" mean that parsing stopped due to the invalid construct
///     of the respective type
///     - @ref Mezzanine::XML::ParseStatus "ParseStatus::StatusBadStartElement" means that parsing stopped because starting tag either had no closing \> symbol or contained some incorrect
///     symbol
///     - @ref Mezzanine::XML::ParseStatus "ParseStatus::StatusBadAttribute" means that parsing stopped because there was an incorrect attribute, such as an attribute without value or
///     with value that is not quoted (note that \<node attr=1\> is incorrect in XML)
///     - @ref Mezzanine::XML::ParseStatus "ParseStatus::StatusBadEndElement" means that parsing stopped because ending tag had incorrect syntax (i.e. extra non-whitespace symbols between
///     tag name and \>)
///     - @ref Mezzanine::XML::ParseStatus "ParseStatus::StatusEndElementMismatch" means that parsing stopped because the closing tag did not match the opening one (i.e. \<node\>\</nedo\>)
///     or because some tag was not closed at all.
///
/// Mezzanine::XML::ParseResult::Description member function can be used to convert parsing status to a string; the returned message is always in English, so you'll have to write your own
/// function if you need a localized string. However please note that the exact messages returned by the Description() function may change from version to version, so any complex
/// status handling should be based on the Status value.
/// \n \n
/// If parsing failed because the source data was not a valid XML, the resulting tree is not destroyed - despite the fact that load function returns error, you can use the part of the
/// tree that was successfully parsed. Obviously, the last element may have an unexpected name/value; for example, if the attribute value does not end with the necessary quotation
/// mark, like in <node attr="value>some data</node> example, the value of attribute attr will contain the string "value\>some data\</node\>".
/// \n \n
/// In addition to the Status code, Mezzanine::XML::ParseResult has an Offset member, which contains the offset of last successfully parsed character if parsing failed because of an error
/// in source data; otherwise offset is 0. For parsing efficiency reasons, Mezzanine::xml does not track the current line during parsing; this offset is in units of Mezzanine::XML::char_t
/// (bytes for character mode, wide characters for wide character mode). Many text editors support 'Go To Position' feature - you can use it to locate the exact error position.
/// Alternatively, if you're loading the document from memory, you can display the error chunk along with the error description.
/// @warning Offset is calculated in the XML buffer in native encoding; if encoding conversion is performed during parsing, offset can not be used to reliably track the error position.
///
/// Mezzanine::XML::ParseResult also has a DocumentEncoding member, which can be used to check that the source data encoding was correctly guessed. It is equal to the exact encoding used
/// during parsing (i.e. with the exact endianness); see @ref XMLLoadingEncodings for more information.
/// \n \n
/// A Mezzanine::XML::ParseResult object can be implicitly converted to bool; if you do not want to handle parsing errors thoroughly, you can just check the return value of load functions
/// as if it was a bool: if (doc.load_file("file.xml")) { //on Successful parse } else { //on failed parse }. A True is returned if parsing was successful.
/// \n \n
/// This is an example of handling loading errors:
/// @code
/// const char Source[] = "<mesh name='sphere'><bounds>0 0 1 1</bounds></mesh>";
/// Mezzanine::XML::Document Doc;
/// Mezzanine::XML::ParseResult Result = Doc.Load(Source);
///
/// if (Result)
///     std::cout << "XML [" << Source << "] parsed without errors\n\n";
/// else
/// {
///     std::cout << "XML [" << Source << "] parsed with errors\n";
///     std::cout << "Error description: " << Result.Description() << "\n";
///     std::cout << "Error offset: " << Result.Offset << " (error at [..." << (Source + Result.Offset) << "]\n\n";
/// }
/// @endcode
/// @subsection XMLLoadingParsingOptions Parsing Options
/// All document loading functions accept the optional parameter options. This is a bitmask that customizes the parsing process: you can select the node types that are parsed and
/// various transformations that are performed with the XML text. Disabling certain transformations can improve parsing performance for some documents; however, the code for all
/// transformations is very well optimized, and thus the majority of documents won't get any performance benefit. As a general rule, only modify parsing flags if you want to get some
/// nodes in the document that are excluded by default (i.e. declaration or comment nodes).
/// @note You should use the usual bitwise arithmetic to manipulate the bitmask: to enable a flag, use mask | flag; to disable a flag, use mask & ~flag.
///
/// These flags control the resulting tree contents:
///     - @ref Mezzanine::XML::ParseDeclaration determines if XML document declaration (node with type @ref Mezzanine::XML::NodeType "NodeDeclaration" ) is to be put in DOM tree. If this flag
///     is off, it is not put in the tree, but is still parsed and checked for correctness. This flag is off by default.
///     - @ref Mezzanine::XML::ParseDocType determines if XML document type declaration (node with type @ref Mezzanine::XML::NodeType "NodeDocType" ) is to be put in DOM tree. If this flag is
///     off, it is not put in the tree, but is still parsed and checked for correctness. This flag is off by default.
///     - @ref Mezzanine::XML::ParsePi determines if processing instructions (nodes with type @ref Mezzanine::XML::NodeType "NodePi" ) are to be put in DOM tree. If this flag is off, they are
///     not put in the tree, but are still parsed and checked for correctness. Note that "<?xml ...?>" (document declaration) is not considered to be a PI. This flag is off by default.
///     - @ref Mezzanine::XML::ParseComments determines if comments (nodes with type @ref Mezzanine::XML::NodeType "NodeComment" ) are to be put in DOM tree. If this flag is off, they are not
///     put in the tree, but are still parsed and checked for correctness. This flag is off by default.
///     - @ref Mezzanine::XML::ParseCdata determines if CDATA sections (nodes with type @ref Mezzanine::XML::NodeType "NodeCdata" ) are to be put in DOM tree. If this flag is off, they are not
///     put in the tree, but are still parsed and checked for correctness. This flag is on by default.
///     - @ref Mezzanine::XML::ParseWsPcdata determines if PCDATA nodes (nodes with type @ref Mezzanine::XML::NodeType "NodePcdata" ) that consist only of whitespace characters are to be put
///     in DOM tree. Often whitespace-only data is not significant for the application, and the cost of allocating and storing such nodes (both memory and speed-wise) can be
///     significant. For example, after parsing XML string "<node> <a/> </node>, <node>" element will have three children when Mezzanine::XML::ParseWsPcdata is set (child with type
///     @ref Mezzanine::XML::NodeType "NodePcdata" and value " ", child with type @ref Mezzanine::XML::NodeType "NodeElement" and name "a", and another child with type
///     @ref Mezzanine::XML::NodeType "NodePcdata" and value " "), and only one child when Mezzanine::XML::ParseWsPcdata is not set. This flag is off by default.
///
/// These flags control the transformation of tree element contents:
///     - @ref Mezzanine::XML::ParseEscapes determines if character and entity references are to be expanded during the parsing process. Character references have the form \&#...; or
///     \&#x...; (... is Unicode numeric representation of character in either decimal ( \&#...; ) or hexadecimal ( \&#x...; ) form), entity references are \&lt;, \&gt;, \&amp;,
///     \&apos; and \&quot; (note that as Mezzanine::xml does not handle DTD, the only allowed entities are predefined ones). If character/entity reference can not be expanded, it is
///     left as is, so you can do additional processing later. Reference expansion is performed on attribute values and PCDATA content. This flag is on by default.
///     - @ref Mezzanine::XML::ParseEol determines if EOL handling (that is, replacing sequences 0x0d 0x0a by a single 0x0a character, and replacing all standalone 0x0d characters by
///     0x0a) is to be performed on input data (that is, comments contents, PCDATA/CDATA contents and attribute values). If this is set all \\r \\n will replaced with \\n.
///     This flag is on by default.
///     - @ref Mezzanine::XML::ParseWconvAttribute determines if attribute value normalization should be performed for all attributes. This means, that whitespace characters (new line,
///     tab and space) are replaced with space (' '). New line characters are always treated as if Mezzanine::XML::ParseEol is set, i.e. \\r \\n is converted to a single space. This
///     flag is on by default.
///     - @ref Mezzanine::XML::ParseWnormAttribute determines if extended attribute value normalization should be performed for all attributes. This means, that after attribute values
///     are normalized as if Mezzanine::XML::ParseWconvAttribute was set, leading and trailing space characters are removed, and all sequences of space characters are replaced by a
///     single space character. The value of Mezzanine::XML::ParseWconvAttribute was set has no effect if this flag is on. This flag is off by default.
///
/// @note Mezzanine::XML::ParseWconvAttribute option performs transformations that are required by W3C specification for attributes that are declared as CDATA; ParseWnormAttribute performs
/// transformations required for NMTOKENS attributes. In the absence of document type declaration all attributes should behave as if they are declared as CDATA, thus
/// Mezzanine::XML::ParseWconvAttribute is the default option.
///
/// Additionally there are three predefined option masks:
///     - Mezzanine::XML::ParseMinimal has all options turned off. This option mask means that Mezzanine::xml does not add declaration nodes, document type declaration nodes, PI nodes,
///     CDATA sections and comments to the resulting tree and does not perform any conversion for input data, so theoretically it is the fastest mode. However, as mentioned above,
///     in practice Mezzanine::XML::ParseDefault is usually equally fast.
///     - Mezzanine::XML::ParseDefault is the default set of flags, i.e. it has all options set to their default values. It includes parsing CDATA sections (comments/PIs are not parsed),
///     performing character and entity reference expansion, replacing whitespace characters with spaces in attribute values and performing EOL handling. Note, that PCDATA sections
///     consisting only of whitespace characters are not parsed (by default) for performance reasons.
///     - Mezzanine::XML::ParseFull is the set of flags which adds nodes of all types to the resulting tree and performs default conversions for input data. It includes parsing CDATA
///     sections, comments, PI nodes, document declaration node and document type declaration node, performing character and entity reference expansion, replacing whitespace
///     characters with spaces in attribute values and performing EOL handling. Note, that PCDATA sections consisting only of whitespace characters are not parsed in this mode.
///
/// This is an example of using different parsing options:
/// @code
/// const char* Source = "<!--comment--><node>&lt;</node>";
///
///  // Parsing with default options; note that comment node is not added to the tree, and entity reference &lt; is expanded
/// Doc.Load(Source);
/// std::cout << "First node value: [" << Doc.GetFirstChild().Value() << "], node child value: [" << Doc.ChildValue("node") << "]\n";
///
/// // Parsing with additional value option; comment node is now added to the tree
/// Doc.Load(Source, Mezzanine::XML::ParseDefault | Mezzanine::XML::ParseComments);
/// std::cout << "First node value: [" << Doc.GetFirstChild().Value() << "], node child value: [" << Doc.ChildValue("node") << "]\n";
///
/// // Parsing with additional value option and without the (default) parse_escapes option; &lt; is not expanded
/// Doc.Load(Source, (Mezzanine::XML::ParseDefault | Mezzanine::XML::ParseComments) & ~Mezzanine::XML::ParseEscapes);
/// std::cout << "First node value: [" << Doc.GetFirstChild().Value() << "], node child value: [" << Doc.ChildValue("node") << "]\n";
///
/// // Parsing with minimal option mask; comment node is not added to the tree, and &lt; is not expanded
/// Doc.Load(Source, Mezzanine::XML::ParseMinimal);
/// std::cout << "First node value: [" << Doc.GetFirstChild().Value() << "], node child value: [" << Doc.ChildValue("node") << "]\n";
/// @endcode
/// @subsection XMLLoadingEncodings Encodings
/// Mezzanine::xml supports all popular Unicode encodings (UTF-8, UTF-16 (big and little endian), UTF-32 (big and little endian); UCS-2 is naturally supported since it's a strict subset
/// of UTF-16) and handles all encoding conversions. Most loading functions accept the optional parameter encoding. This is a value of enumeration type Mezzanine::XML::Encoding, that
/// can have the following values:
///     - @ref Mezzanine::XML::Encoding "Encoding::EncodingAuto" means that pugixml will try to guess the encoding based on source XML data. The algorithm is a modified version of the
///     one presented in Appendix F.1 of XML recommendation; it tries to match the first few bytes of input data with the following patterns in strict order:
///             - If first four bytes match UTF-32 BOM (Byte Order Mark), encoding is assumed to be UTF-32 with the endianness equal to that of BOM;
///             - If first two bytes match UTF-16 BOM, encoding is assumed to be UTF-16 with the endianness equal to that of BOM;
///             - If first three bytes match UTF-8 BOM, encoding is assumed to be UTF-8;
///             - If first four bytes match UTF-32 representation of \<, encoding is assumed to be UTF-32 with the corresponding endianness;
///             - If first four bytes match UTF-16 representation of \<?, encoding is assumed to be UTF-16 with the corresponding endianness;
///             - If first two bytes match UTF-16 representation of \<, encoding is assumed to be UTF-16 with the corresponding endianness (this guess may yield incorrect result,
///             but it's better than UTF-8);
///             - Otherwise encoding is assumed to be UTF-8.
///     - @ref Mezzanine::XML::Encoding "Encoding::EncodingUTF8" corresponds to UTF-8 encoding as defined in the Unicode standard; UTF-8 sequences with length equal to 5 or 6 are not
///     standard and are rejected.
///     - @ref Mezzanine::XML::Encoding "Encoding::EncodingUTF16LE" corresponds to little-endian UTF-16 encoding as defined in the Unicode standard; surrogate pairs are supported.
///     - @ref Mezzanine::XML::Encoding "Encoding::EncodingUTF16BE" corresponds to big-endian UTF-16 encoding as defined in the Unicode standard; surrogate pairs are supported.
///     - @ref Mezzanine::XML::Encoding "Encoding::EncodingUTF16" corresponds to UTF-16 encoding as defined in the Unicode standard; the endianness is assumed to be that of the target
///     platform.
///     - @ref Mezzanine::XML::Encoding "Encoding::EncodingUTF32LE" corresponds to little-endian UTF-32 encoding as defined in the Unicode standard.
///     - @ref Mezzanine::XML::Encoding "Encoding::EncodingUTF32BE" corresponds to big-endian UTF-32 encoding as defined in the Unicode standard.
///     - @ref Mezzanine::XML::Encoding "Encoding::EncodingUTF32" corresponds to UTF-32 encoding as defined in the Unicode standard; the endianness is assumed to be that of the target
///     platform.
///     - @ref Mezzanine::XML::Encoding "Encoding::Encodingwchar_t " corresponds to the encoding of Mezzanine::XML::wchar_t type; it has the same meaning as either
///     @ref Mezzanine::XML::Encoding "EncodingUTF16" or @ref Mezzanine::XML::Encoding "EncodingUTF32", depending on Mezzanine::XML::wchar_t size.
///
/// The algorithm used for @ref Mezzanine::XML::Encoding "EncodingAuto" correctly detects any supported Unicode encoding for all well-formed XML documents (since they start with document
/// declaration) and for all other XML documents that start with \<; if your XML document does not start with \< and has encoding that is different from UTF-8, use the specific encoding.
/// @note The current behavior for Unicode conversion is to skip all invalid UTF sequences during conversion. This behavior should not be relied upon; moreover, in case no encoding
/// conversion is performed, the invalid sequences are not removed, so you'll get them as is in node/attribute contents.
///
/// @subsection XMLLoadingStandards Conformance to W3C Specification
/// Mezzanine::xml is not fully W3C conformant - it can load any valid XML document, but does not perform some well-formedness checks. While considerable effort is made to reject invalid
/// XML documents, some validation is not performed because of performance reasons.
/// \n \n There is only one non-conformant behavior when dealing with valid XML documents, Mezzanine::xml does not use information supplied in document type declaration for parsing. This
/// means that entities declared in DOCTYPE are not expanded, and all attribute/PCDATA values are always processed in a uniform way that depends only on parsing options.
/// \n \n
/// As for rejecting invalid XML documents, there are a number of incompatibilities with W3C specification, including:
///     - Multiple attributes of the same node can have equal names.
///     - All non-ASCII characters are treated in the same way as symbols of English alphabet, so some invalid tag names are not rejected.
///     - Attribute values which contain \< are not rejected.
///     - Invalid entity/character references are not rejected and are instead left as is.
///     - Comment values can contain --.
///     - XML data is not required to begin with document declaration; additionally, document declaration can appear after comments and other nodes.
///     - Invalid document type declarations are silently ignored in some cases.
///
/// @section XMLAccessing Accessing Document Data
///     - @ref XMLAccessingBasics
///     - @ref XMLAccessingNodeData
///     - @ref XMLAccessingAttributeData
///     - @ref XMLAccessingContentBased
///     - @ref XMLAccessingIterators
///     - @ref XMLAccessingWalker
///     - @ref XMLAccessingPredicates
///     - @ref XMLAccessingMisc
///
/// Mezzanine::xml features an extensive interface for getting various types of data from the document and for traversing the document. This section provides documentation for all such
/// functions that do not modify the tree except for XPath-related functions; see @ref XMLXPath for XPath reference. As discussed in @ref XMLInterface, there are two types of handles
/// to tree data - Mezzanine::XML::Node and Mezzanine::XML::Attribute. The handles have special null (empty) values which propagate through various functions and thus are useful for writing
/// more concise code; see @ref XMLInterface for details. The documentation in this section will explicitly state the results of all function in case of null inputs.
/// @subsection XMLAccessingBasics Basic traversal functions
/// The internal representation of the document is a tree, where each node has a list of child nodes (the order of children corresponds to their order in the XML representation),
/// additionally element nodes have a list of attributes, which is also ordered. Several functions are provided in order to let you get from one node in the tree to the other. These
/// functions roughly correspond to the internal representation, and thus are usually building blocks for other methods of traversing (i.e. XPath traversals are based on these
/// functions).
/// @code
/// Mezzanine::XML::Node Mezzanine::XML::Node::GetParent() const;
/// Mezzanine::XML::Node Mezzanine::XML::Node::GetFirstChild() const;
/// Mezzanine::XML::Node Mezzanine::XML::Node::GetLastChild() const;
/// Mezzanine::XML::Node Mezzanine::XML::Node::GetNextSibling() const;
/// Mezzanine::XML::Node Mezzanine::XML::Node::GetPreviousSibling() const;
///
/// Mezzanine::XML::Attribute Mezzanine::XML::Node::GetFirstAttribute() const;
/// Mezzanine::XML::Attribute Mezzanine::XML::Node::GetLastAttribute() const;
/// Mezzanine::XML::Attribute Mezzanine::XML::Attribute::GetNextAttribute() const;
/// Mezzanine::XML::Attribute Mezzanine::XML::Attribute::GetPreviousAttribute() const;
/// @endcode
/// The GetParent function returns the node's parent; all non-null nodes except the document have non-null parent. GetFirstChild and GetLastChild return the first and last child of the
/// node, respectively; note that only document nodes and element nodes can have non-empty child node list. If node has no children, both functions return null nodes. GetNextSibling
/// and GetPreviousSibling return the node that's immediately to the right/left of this node in the children list, respectively - for example, in \<a/> \<b/> \<c/>, calling GetNextSibling
/// for a handle that points to \<b/> results in a handle pointing to \<c/>, and calling GetPreviousSibling results in handle pointing to \<a/>. If node does not have next/previous sibling
/// (this happens if it is the last/first node in the list, respectively), the functions return null nodes. GetFirstAttribute, GetLastAttribute, GetNextAttribute and GetPreviousAttribute
/// functions behave similarly to the corresponding child node functions and allow to iterate through attribute list in the same way.
/// @note Because of memory consumption reasons, attributes do not have a link to their parent nodes. Thus there is no Mezzanine::XML::attribute::parent() function.
///
/// Calling any of the functions above on the null handle results in a null handle - i.e. node.GetFirstChild().GetNextSibling() returns the second child of node, and null handle if node
/// is null, has no children at all or if it has only one child node.
/// \n \n
/// With these functions, you can iterate through all child nodes and display all attributes like this:
/// @code
/// for (Mezzanine::XML::Node tool = tools.GetFirstChild(); tool; tool = tool.GetNextSibling())
/// {
///     std::cout << "Tool:";
///
///     for (Mezzanine::XML::Attribute attr = tool.GetFirstAttribute(); attr; attr = attr.GetNextAttribute())
///     {
///         std::cout << " " << attr.Name() << "=" << attr.Value();
///     }
///
///     std::cout << std::endl;
/// }
/// @endcode
/// @subsection XMLAccessingNodeData Getting Node Data
/// Apart from structural information (parent, child nodes, attributes), nodes can have name and value, both of which are strings. Depending on node type, name or value may be absent.
/// @ref Mezzanine::XML::NodeType "NodeDocument" nodes do not have a name or value, @ref Mezzanine::XML::NodeType "NodeElement" and @ref Mezzanine::XML::NodeType "NodeType::NodeDeclaration" nodes
/// always have a name but never have a value, @ref Mezzanine::XML::NodeType "NodeType::NodePcdata", @ref Mezzanine::XML::NodeType "NodeType::NodeCdata",
/// @ref Mezzanine::XML::NodeType "NodeType::NodeComment" and @ref Mezzanine::XML::NodeType "NodeType::NodeDocType" nodes never have a name but always have a value (it may be empty though),
/// @ref Mezzanine::XML::NodeType "NodeType::NodePi" nodes always have a name and a value (again, value may be empty). In order to get node's name or value, you can use the following
/// functions:
/// @code
/// const char_t* Mezzanine::XML::Node::Name() const;
/// const char_t* Mezzanine::XML::Node::Value() const;
/// @endcode
/// In case node does not have a name or value or if the node handle is null, both functions return empty strings - they never return null pointers.
/// \n \n
/// It is common to store data as text contents of some node - i.e. \<node> \<description> This is a node \</description> \</node>. In this case, \<description> node does not have
/// a value, but instead has a child of type @ref Mezzanine::XML::NodeType "NodeType::NodePcdata" with value "This is a node". Mezzanine::xml provides two helper functions to parse such data:
/// @code
/// const char_t* Mezzanine::XML::Node::ChildValue() const;
/// const char_t* Mezzanine::XML::Node::ChildValue(const char_t* name) const;
/// @endcode
/// ChildValue() returns the value of the first child with type @ref Mezzanine::XML::NodeType "NodeType::NodePcdata" or @ref Mezzanine::XML::NodeType "NodeType::NodeCdata";
/// ChildValue(Name) is a simple wrapper for Child(Name).ChildValue(). For the above example, calling node.ChildValue("description") and description.ChildValue() will both produce
/// string "This is a node". If there is no child with relevant type, or if the handle is null, ChildValue functions return empty string.
/// \n \n
/// There is an example of using some of these functions @ref XMLAccessingAttributeData "at the end of the next section".
/// @subsection XMLAccessingAttributeData Getting attribute data
/// All attributes have name and value, both of which are strings (value may be empty). There are two corresponding accessors:
/// @code
/// const char_t* Mezzanine::XML::Attribute::Name() const;
/// const char_t* Mezzanine::XML::Attribute::Value() const;
/// @endcode
/// In case the attribute handle is null, both functions return empty strings - they never return null pointers.
/// \n \n
/// In many cases attribute values have types that are not strings - i.e. an attribute may always contain values that should be treated as integers, despite the fact that they are
/// represented as strings in XML. Mezzanine::xml provides several accessors that convert attribute value to some other type:
/// @code
/// // Native C++ types
/// bool Mezzanine::XML::Attribute::AsBool() const;
/// double Mezzanine::XML::Attribute::AsDouble() const;
/// float Mezzanine::XML::Attribute::AsFloat() const;
/// int Mezzanine::XML::Attribute::AsInt() const;
/// unsigned int Mezzanine::XML::Attribute::AsUint() const;
///
/// //Mezzanine abstracted types
/// Real Mezzanine::XML::Attribute::AsReal() const;
/// String Mezzanine::XML::Attribute::AsString() const;
/// Whole Mezzanine::XML::Attribute::AsWhole() const;
/// @endcode
/// AsDouble, AsFloat, AsInt, AsUint, AsReal and AsWhole convert attribute values to numbers. If attribute handle is null or attribute value is empty, 0 is returned. Otherwise, all
/// leading whitespace characters are truncated, and the remaining string is parsed as a decimal number (AsInt, AsUint or AsWhole) or as a floating point number in either decimal
/// or scientific form (AsDouble, AsFloat or AsReal). Any extra characters are silently discarded, i.e. AsInt will return 1 for string "1abc".
/// \n \n
/// In case the input string contains a number that is out of the target numeric range, the result is undefined.
/// @warning Some number conversion functions depend on current C locale as set with setlocale, so may return unexpected results if the locale is different from "C".
///
/// AsBool converts attribute value to boolean as follows: if attribute handle is null or attribute value is empty, false is returned. Otherwise, true is returned if the first
/// character is one of '1', 't', 'T', 'y', 'Y'. This means that strings like "true" and "yes" are recognized as true, while strings like "false" and "no" are recognized as false.
/// For more complex matching you'll have to write your own function.
/// @note There are no portable 64-bit types in C++, so there is no corresponding conversion function. If your platform has a 64-bit integer, you can easily write a conversion
/// function yourself. We are planning on addressing this with a new Mezzanine abstracted type, but we have not done so yet.
///
/// This is an example of using these functions, along with node data retrieval ones:
/// @code
/// for (Mezzanine::XML::Node tool = tools.GetChild("Tool"); tool; tool = tool.GetNextSibling("Tool"))
/// {
///     std::cout << "Tool " << tool.GetAttribute("Filename").Value();
///     std::cout << ": AllowRemote " << tool.GetAttribute("AllowRemote").AsBool();
///     std::cout << ", Timeout " << tool.GetAttribute("Timeout").AsInt();
///     std::cout << ", Description '" << tool.ChildValue("Description") << "'\n";
/// }
/// @endcode
/// @subsection XMLAccessingContentBased Content Based Traversal Functions
/// Since a lot of document traversal consists of finding the node/attribute with the correct name, there are special functions for that purpose:
/// @code
/// Mezzanine::XML::Node Mezzanine::XML::Node::GetChild(const char_t* name) const;
/// Mezzanine::XML::Attribute Mezzanine::XML::Node::GetAttribute(const char_t* name) const;
/// Mezzanine::XML::Node Mezzanine::XML::Node::GetNextSibling(const char_t* name) const;
/// Mezzanine::XML::Node Mezzanine::XML::Node::GetPreviousSibling(const char_t* name) const;
/// @endcode
/// GetChild and GetAttribute return the first child/attribute with the specified name; GetNextSibling and GetPreviousSibling return the first sibling in the corresponding direction
/// with the specified name. All string comparisons are case-sensitive. In case the node handle is null or there is no node/attribute with the specified name, null handle is returned.
/// \n \n
/// GetChild and GetNextSibling functions can be used together to loop through all child nodes with the desired name like this:
/// @code
/// for (Mezzanine::XML::Node tool = tools.GetChild("Tool"); tool; tool = tool.GetNextSibling("Tool"))
///     {}
/// @endcode
/// Occasionally the needed node is specified not by the unique name but instead by the value of some attribute; for example, it is common to have node collections with each node
/// having a unique id: \<group> \<item id="1"/> \<item id="2"/> \</group>. There are two functions for finding child nodes based on the attribute values:
/// @code
/// Mezzanine::XML::Node Mezzanine::XML::Node::FindChildbyAttribute(const char_t* name, const char_t* attr_name, const char_t* attr_value) const;
/// Mezzanine::XML::Node Mezzanine::XML::Node::FindChildbyAttribute(const char_t* attr_name, const char_t* attr_value) const;
/// @endcode
/// The three-argument function returns the first child node with the specified name which has an attribute with the specified name/value; the two-argument function skips the name
/// test for the node, which can be useful for searching in heterogeneous collections. If the node handle is null or if no node is found, null handle is returned. All string
/// comparisons are case-sensitive.
/// \n \n
/// In all of the above functions, all arguments have to be valid strings; passing null pointers results in undefined behavior.
///
/// This is an example of using these functions:
/// @code
/// std::cout << "Tool for *.dae generation: " << tools.FindChildbyAttribute("Tool", "OutputFileMasks", "*.dae").GetAttribute("Filename").Value() << "\n";
///
/// for (Mezzanine::XML::Node tool = tools.GetChild("Tool"); tool; tool = tool.GetNextSibling("Tool"))
/// {
///     std::cout << "Tool " << tool.GetAttribute("Filename").Value() << "\n";
/// }
/// @endcode
/// @subsection XMLAccessingIterators Traversing Node/Attribute Lists via Iterators
/// Child node lists and attribute lists are simply double-linked lists; while you can use GetPreviousSibling/GetNextSibling and other such functions for iteration, Mezzanine::xml
/// additionally provides node and attribute iterators, so that you can treat nodes as containers of other nodes or attributes:
/// @code
/// class Mezzanine::XML::NodeIterator;
/// class Mezzanine::XML::AttributeIterator;
///
/// typedef Mezzanine::XML::NodeIterator Mezzanine::XML::Node::iterator;
/// iterator Mezzanine::XML::Node::begin() const;
/// iterator Mezzanine::XML::Node::end() const;
///
/// typedef Mezzanine::XML::AttributeIterator Mezzanine::XML::Node::attribute_iterator;
/// attribute_iterator Mezzanine::XML::Node::attributes_begin() const;
/// attribute_iterator Mezzanine::XML::Node::attributes_end() const;
/// @endcode
/// begin and attributes_begin return iterators that point to the first node/attribute, respectively; end and attributes_end return past-the-end iterator for node/attribute list,
/// respectively - this iterator can't be dereferenced, but decrementing it results in an iterator pointing to the last element in the list (except for empty lists, where decrementing
/// past-the-end iterator results in undefined behavior). Past-the-end iterator is commonly used as a termination value for iteration loops (see sample below). If you want to get an
/// iterator that points to an existing handle, you can construct the iterator with the handle as a single constructor argument, like so: Mezzanine::XML::NodeIterator(node). For
/// Mezzanine::XML::AttributeIterator, you'll have to provide both an attribute and its parent node.
/// \n \n
/// begin and end return equal iterators if called on null node; such iterators can't be dereferenced. attributes_begin and attributes_end behave the same way. For correct iterator
/// usage this means that child node/attribute collections of null nodes appear to be empty.
/// @note The names begin, end, attribute_begin and attribute_end were chosen for maximum compatibility. Some template functions that act on containers rely on the name begin and end.
/// We may develop a naming scheme that fits better into the existing API in the future.
///
/// Both types of iterators have bidirectional iterator semantics (i.e. they can be incremented and decremented, but efficient random access is not supported) and support all usual
/// iterator operations - comparison, dereference, etc. The iterators are invalidated if the node/attribute objects they're pointing to are removed from the tree; adding
/// nodes/attributes does not invalidate any iterators.
/// \n \n
/// Here is an example of using iterators for document traversal:
/// @code
/// for (Mezzanine::XML::NodeIterator it = tools.begin(); it != tools.end(); ++it)
/// {
///     std::cout << "Tool:";
///
///     for (Mezzanine::XML::AttributeIterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait)
///     {
///         std::cout << " " << ait->Name() << "=" << ait->Value();
///     }
///
///     std::cout << std::endl;
/// }
/// @endcode
/// @warning Node and attribute iterators are somewhere in the middle between const and non-const iterators. While dereference operation yields a non-constant reference to the object,
/// so that you can use it for tree modification operations, modifying this reference by assignment - i.e. passing iterators to a function like std::sort - will not give expected results,
/// as assignment modifies local handle that's stored in the iterator.
///
/// @subsection XMLAccessingWalker Recursive traversal with Mezzanine::XML::TreeWalker
/// The methods described above allow traversal of immediate children of some node; if you want to do a deep tree traversal, you'll have to do it via a recursive function or some
/// equivalent method. However, pugixml provides a helper for depth-first traversal of a subtree. In order to use it, you have to implement the Mezzanine::XML::TreeWalker interface and to
/// call the Mezzanine::XML::Node::Traverse function.
/// @code
/// bool Mezzanine::XML::Node::Traverse(Mezzanine::XML::TreeWalker& walker);
/// @endcode
/// The traversal is launched by calling traverse function on traversal root and proceeds as follows:
///     - First, TreeWalker::begin function is called with traversal root as its argument.
///     - Then, TreeWalker::for_each function is called for all nodes in the traversal subtree in depth first order, excluding the traversal root. Node is passed as an argument.
///     - Finally, TreeWalker::end function is called with traversal root as its argument.
///
/// @warning The Names of the Treewalker could change without notice prior to the initial of the engine.
///
/// If begin, end or any of the for_each calls return false, the traversal is terminated and false is returned as the traversal result; otherwise, the traversal results in true.
/// Note that you don't have to override begin or end functions; their default implementations return true.
/// \n \n
/// You can get the node's depth relative to the traversal root at any point by calling TreeWalker::Depth function. It returns -1 if called from begin/end, and returns 0-based depth if
/// called from TreeWalker::for_each - depth is 0 for all children of the traversal root, 1 for all grandchildren and so on.
/// \n \n
/// This is an example of traversing tree hierarchy with Mezzanine::XML::TreeWalker:
/// @code
/// struct SimpleWalker: Mezzanine::XML::TreeWalker
/// {
///     virtual bool for_each(Mezzanine::XML::Node& node)
///     {
///         for (int i = 0; i < Depth(); ++i) std::cout << "  "; // indentation
///
///         std::cout << ": Name='" << node.Name() << "', Value='" << node.Value() << "'\n";
///
///         return true; // continue traversal
///     }
/// };
/// @endcode
/// @code
/// SimpleWalker walker;
/// Doc.Traverse(walker);
/// @endcode
/// @subsection XMLAccessingPredicates Searching for Nodes/Attributes with Predicates
/// While there are existing functions for getting a node/attribute with known contents, they are often not sufficient for simple queries. As an alternative for manual iteration
/// through nodes/attributes until the needed one is found, you can make a predicate and call one of Find functions:
/// @code
/// template <typename Predicate> Mezzanine::XML::Attribute Mezzanine::XML::Node::FindAttribute(Predicate pred) const;
/// template <typename Predicate> Mezzanine::XML::Node Mezzanine::XML::Node::FindChild(Predicate pred) const;
/// template <typename Predicate> Mezzanine::XML::Node Mezzanine::XML::Node::FindNode(Predicate pred) const;
/// @endcode
/// The predicate should be either a plain function or a function object which accepts one argument of type Mezzanine::XML::Attribute (for FindAttribute) or Mezzanine::XML::Node (for FindChild
/// and FindNode), and returns bool. The predicate is never called with null handle as an argument.
/// \n \n
/// Mezzanine::XML::Node::FindAttribute function iterates through all attributes of the specified node, and returns the first attribute for which the predicate returned true. If the
/// predicate returned false for all attributes or if there were no attributes (including the case where the node is null), null attribute is returned.
/// \n \n
/// Mezzanine::XML::Node::FindChild function iterates through all child nodes of the specified node, and returns the first node for which the predicate returned true. If the predicate
/// returned false for all nodes or if there were no child nodes (including the case where the node is null), null node is returned.
/// \n \n
/// Mezzanine::XML::Node::FindNode function performs a depth-first traversal through the subtree of the specified node (excluding the node itself), and returns the first node for which
/// the predicate returned true. If the predicate returned false for all nodes or if subtree was empty, null node is returned.
/// \n \n
/// Here are some sample predicates:
/// @code
/// bool SmallTimeout(Mezzanine::XML::Node node)
/// {
///     return node.GetAttribute("Timeout").AsInt() < 20;
/// }
///
/// struct AllowRemotePredicate
/// {
///     bool operator()(Mezzanine::XML::Attribute attr) const
///     {
///         return strcmp(attr.Name(), "AllowRemote") == 0;
///     }
///
///     bool operator()(Mezzanine::XML::Node node) const
///     {
///         return node.GetAttribute("AllowRemote").AsBool();
///     }
/// };
/// @endcode
/// This is an example of using predicate-based functions:
/// @code
/// // Find child via predicate (looks for direct children only)
/// std::cout << tools.FindChild(AllowRemotePredicate()).GetAttribute("Filename").Value() << std::endl;
///
/// // Find node via predicate (looks for all descendants in depth-first order)
/// std::cout << doc.FindNode(AllowRemotePredicate()).GetAttribute("Filename").Value() << std::endl;
///
/// // Find attribute via predicate
/// std::cout << tools.LastChild().FindAttribute(AllowRemotePredicate()).Value() << std::endl;
///
/// // We can use simple functions instead of function objects
/// std::cout << tools.FindChild(SmallTimeout).GetAttribute("Filename").Value() << std::endl;
/// @endcode
/// @subsection XMLAccessingMisc Miscellaneous functions
/// If you need to get the document root of some node, you can use the following function:
/// @code
/// Mezzanine::XML::Node Mezzanine::XML::Node::GetRoot() const;
/// @endcode
/// This function returns the node with type @ref Mezzanine::XML::NodeType "NodeDocument", which is the root node of the document the node belongs to (unless the node is null, in which
/// case null node is returned).
/// \n \n
/// While Mezzanine::xml supports complex XPath expressions, sometimes a simple path handling facility is needed. There are two functions, for getting node path and for converting path
/// to a node:
/// @code
/// Mezzanine::String Mezzanine::XML::Node::Path(char_t delimiter = '/') const;
/// Mezzanine::XML::Node Mezzanine::XML::Node::FirstElementByPath(const char_t* Path, char_t delimiter = '/') const;
/// @endcode
/// Node paths consist of node names, separated with a delimiter (which is / by default); also paths can contain self (.) and parent (..) pseudo-names, so that this is a valid path:
/// "../../foo/./bar". path returns the path to the node from the document root, FirstElementByPath looks for a node represented by a given path; a path can be an absolute one
/// (absolute paths start with the delimiter), in which case the rest of the path is treated as document root relative, and relative to the given node. For example, in the following
/// document: \<a> \<b> \<c/> \</b> \</a>, node \<c/> has path "a/b/c"; calling FirstElementByPath for document with path "a/b" results in node \<b/>; calling FirstElementByPath for
/// node \<a/> with path "../a/./b/../." results in node \<a/>; calling FirstElementByPath with path "/a" results in node \<a/> for any node.
/// \n \n
/// In case path component is ambiguous (if there are two nodes with given name), the first one is selected; paths are not guaranteed to uniquely identify nodes in a document. If any
/// component of a path is not found, the result of FirstElementByPath is null node; also FirstElementByPath returns null node for null nodes, in which case the path does not matter.
/// path returns an empty string for null nodes.
///
/// Mezzanine::xml does not record row/column information for nodes upon parsing for efficiency reasons. However, if the node has not changed in a significant way since parsing (the
/// name/value are not changed, and the node itself is the original one, i.e. it was not deleted from the tree and re-added later), it is possible to get the offset from the
/// beginning of XML buffer:
/// @code
/// ptrdiff_t Mezzanine::XML::node::OffSetDebug() const;
/// @endcode
/// If the offset is not available (this happens if the node is null, was not originally parsed from a stream, or has changed in a significant way), the function returns -1. Otherwise
/// it returns the offset to node's data from the beginning of XML buffer in Mezzanine::XML::char_t units. For more information on parsing offsets, see parsing error handling documentation.
/// \n \n
/// @section XMLModifying Modifiying Documents
///     - @ref XMLModifyingNodeData
///     - @ref XMLModifyingAttributeData
///     - @ref XMLModifyingAdding
///     - @ref XMLModifyingRemoving
///     - @ref XMLModifyingCloning
///
/// The document in Mezzanine::xml is fully mutable: you can completely change the document structure and modify the data of nodes/attributes. This section provides documentation for the
/// relevant functions. All functions take care of memory management and structural integrity themselves, so they always result in structurally valid tree - however, it is possible to
/// create an invalid XML tree (for example, by adding two attributes with the same name or by setting attribute/node name to empty/invalid string). Tree modification is optimized for
/// performance and for memory consumption, so if you have enough memory you can create documents from scratch with Mezzanine::xml and later save them to file/stream instead of relying on
/// error-prone manual text writing and without too much overhead.
/// \n \n
/// All member functions that change node/attribute data or structure are non-constant and thus can not be called on constant handles. However, you can easily convert constant handle
/// to non-constant one by simple assignment: void foo(const Mezzanine::XML::Node& n) { Mezzanine::XML::Node nc = n; }, so const-correctness here mainly provides additional documentation.
/// \n \n
/// @subsection XMLModifyingNodeData Setting Node Data
///As discussed before, nodes can have name and value, both of which are c-strings. Depending on node type, name or value may be absent. @ref Mezzanine::XML::NodeType "NodeDocument" nodes
/// do not have a name or value, @ref Mezzanine::XML::NodeType "NodeElement" and @ref Mezzanine::XML::NodeType "NodeDeclaration" nodes always have a name but never have a value,
/// @ref Mezzanine::XML::NodeType "NodePcdata", @ref Mezzanine::XML::NodeType "NodeCdata", @ref Mezzanine::XML::NodeType "NodeComment" and @ref Mezzanine::XML::NodeType "NodeDocType" nodes never have a
/// name but always have a value (it may be empty though), @ref Mezzanine::XML::NodeType "NodePi" nodes always have a name and a value (again, value may be empty). In order to set node's
/// name or value, you can use the following functions:
/// @code
/// bool Mezzanine::XML::Node::SetName(const char_t* rhs);
/// bool Mezzanine::XML::Node::SetName(const String& rhs);
/// bool Mezzanine::XML::Node::SetValue(const char_t* rhs);
/// @endcode
/// Both functions try to set the name/value to the specified string, and return the operation result. The operation fails if the node can not have name or value (for instance, when
/// trying to call SetName on a @ref Mezzanine::XML::NodeType "NodePcdata" node), if the node handle is null, or if there is insufficient memory to handle the request. The provided string
/// is copied into document managed memory and can be destroyed after the function returns (for example, you can safely pass stack-allocated buffers to these functions). The name/value
/// content is not verified, so take care to use only valid XML names, or the document may become malformed.
/// \n \n
/// There is no equivalent of ChildValue function for modifying text children of the node.
/// \n \n
/// This is an example of setting node name and value:
/// @code
/// Mezzanine::XML::Node node = doc.GetChild("node");
///
/// // change node name
/// std::cout << node.SetName("notnode");
/// std::cout << ", new node name: " << node.Name() << std::endl;
///
/// // change comment text
/// std::cout << doc.GetLastChild().SetValue("useless comment");
/// std::cout << ", new comment text: " << doc.LastChild().Value() << std::endl;
///
/// // we can't change value of the element or name of the comment
/// std::cout << node.SetValue("1") << ", " << doc.LastChild().SetName("2") << std::endl;
/// @endcode
/// @subsection XMLModifyingAttributeData Setting Attribute Data
/// All attributes have name and value, both of which are strings (value may be empty). You can set them with the following functions:
/// @code
/// bool Mezzanine::XML::Attribute::SetName(const char_t* rhs);
/// bool Mezzanine::XML::Attribute::SetValue(const char_t* rhs);
/// @endcode
/// Both functions try to set the name/value to the specified string, and return the operation result. The operation fails if the attribute handle is null, or if there is insufficient
/// memory to handle the request. The provided string is copied into document managed memory and can be destroyed after the function returns (for example, you can safely pass
/// stack-allocated buffers to these functions). The name/value content is not verified, so take care to use only valid XML names, or the document may become malformed.
/// \n \n
/// In addition to string functions, several functions are provided for handling attributes with numbers and booleans as values:
/// @code
/// bool Mezzanine::XML::Attribute::SetValue(int rhs);
/// bool Mezzanine::XML::Attribute::SetValue(unsigned int rhs);
/// bool Mezzanine::XML::Attribute::SetValue(double rhs);
/// bool Mezzanine::XML::Attribute::SetValue(bool rhs);
/// @endcode
/// The above functions convert the argument to string and then call the base SetValue function. Integers are converted to a decimal form, floating-point numbers are converted to either
/// decimal or scientific form, depending on the number magnitude, boolean values are converted to either "true" or "false".
/// @warning Number conversion functions depend on current C locale as set with setlocale, so may generate unexpected results if the locale is different from "C".
///
/// @note There are no portable 64-bit types in C++, so there is no corresponding SetValue function. If your platform has a 64-bit integer, you can easily write such a function yourself.
///
/// For convenience, all set_value functions have the corresponding assignment operators:
/// @code
/// Mezzanine::XML::Attribute& Mezzanine::XML::Attribute::operator=(const char_t* rhs);
/// Mezzanine::XML::Attribute& Mezzanine::XML::Attribute::operator=(int rhs);
/// Mezzanine::XML::Attribute& Mezzanine::XML::Attribute::operator=(unsigned int rhs);
/// Mezzanine::XML::Attribute& Mezzanine::XML::Attribute::operator=(double rhs);
/// Mezzanine::XML::Attribute& Mezzanine::XML::Attribute::operator=(bool rhs);
/// @endcode
/// These operators simply call the right SetValue function and return the attribute they're called on; the return value of SetValue is ignored, so errors are ignored.
/// \n \n
/// This is an example of setting attribute name and value:
/// @code
/// Mezzanine::XML::Attribute attr = node.GetAttribute("id");
///
/// // change attribute name/value
/// std::cout << attr.SetName("key") << ", " << attr.SetValue("345");
/// std::cout << ", new attribute: " << attr.name() << "=" << attr.value() << std::endl;
///
/// // we can use numbers or booleans
/// attr.SetValue(1.234);
/// std::cout << "new attribute value: " << attr.Value() << std::endl;
///
/// // we can also use assignment operators for more concise code
/// attr = true;
/// std::cout << "final attribute value: " << attr.value() << std::endl;
/// @endcode
/// @subsection XMLModifyingAdding Adding Nodes/Attributes
/// Nodes and attributes do not exist without a document tree, so you can't create them without adding them to some document. A node or attribute can be created at the end of
/// node/attribute list or before/after some other node:
/// @code
/// Mezzanine::XML::Attribute Mezzanine::XML::Node::AppendAttribute(const char_t* Name);
/// Mezzanine::XML::Attribute Mezzanine::XML::Node::AppendAttribute(const String& Name);
/// Mezzanine::XML::Attribute Mezzanine::XML::Node::PrependAttribute(const char_t* Name);
/// Mezzanine::XML::Attribute Mezzanine::XML::Node::InsertAttributeAfter(const char_t* Name, const Mezzanine::XML::Attribute& attr);
/// Mezzanine::XML::Attribute Mezzanine::XML::Node::InsertAttributeBefore(const char_t* Name, const Mezzanine::XML::Attribute& attr);
///
/// Mezzanine::XML::Node Mezzanine::XML::Node::AppendChild(Mezzanine::XML::NodeType type = NodeElement);
/// Mezzanine::XML::Node Mezzanine::XML::Node::PrependChild(Mezzanine::XML::NodeType type = NodeElement);
/// Mezzanine::XML::Node Mezzanine::XML::Node::InsertChildAfter(Mezzanine::XML::NodeType type, const Mezzanine::XML::Node& node);
/// Mezzanine::XML::Node Mezzanine::XML::Node::InsertChildBefore(Mezzanine::XML::NodeType type, const Mezzanine::XML::Node& node);
///
/// Mezzanine::XML::Node Mezzanine::XML::Node::AppendChild(const char_t* Name);
/// Mezzanine::XML::Node Mezzanine::XML::Node::AppendChild(const String& Name);
/// Mezzanine::XML::Node Mezzanine::XML::Node::PrependChild(const char_t* Name);
/// Mezzanine::XML::Node Mezzanine::XML::Node::PrependChild(const String& Name);
/// Mezzanine::XML::Node Mezzanine::XML::Node::InsertChildAfter(const char_t* Name, const Mezzanine::XML::Node& node);
/// Mezzanine::XML::Node Mezzanine::XML::Node::InsertChildBefore(const char_t* Name, const Mezzanine::XML::Node& node);
/// @endcode
/// AppendAttribute and AppendChild create a new node/attribute at the end of the corresponding list of the node the method is called on; PrependAttribute and PrependChild create a new
/// node/attribute at the beginning of the list; InsertAttributeAfter, InsertAttributeBefore, InsertChildAfter and InsertAttributeBefore add the node/attribute before or after the
/// specified node/attribute.
/// \n \n
/// The overloads of AppendChild and PrependChild that accept Mezzanine::String references convert it to a string, then call the version of the function that accepts a c-style string.
/// \n \n
/// Attribute functions create an attribute with the specified name; you can specify the empty name and change the name later if you want to. Node functions with the type argument
/// create the node with the specified type; since node type can't be changed, you have to know the desired type beforehand. Also note that not all types can be added as children; see
/// below for clarification. Node functions with the name argument create the element node (NodeElement) with the specified name.
/// \n \n
/// All functions return the handle to the created object on success, and null handle on failure. There are several reasons for failure:
///     - Adding fails if the target node is null;
///     - Only @ref Mezzanine::XML::NodeType "NodeType::NodeElement" nodes can contain attributes, so attribute adding fails if node is not an element;
///     - Only @ref Mezzanine::XML::NodeType "NodeType::NodeDocument" and @ref Mezzanine::XML::NodeType "NodeType::NodeElement" nodes can contain children, so child node adding fails if the
///     target node is not an element or a document;
///     - @ref Mezzanine::XML::NodeType "NodeType::NodeDocument" and @ref Mezzanine::XML::NodeType "NodeType::NodeNull" nodes can not be inserted as children, so passing
///     @ref Mezzanine::XML::NodeType "NodeType::NodeDocument" or @ref Mezzanine::XML::NodeType "NodeType::NodeNull value as type results in operation failure;
///     - @ref Mezzanine::XML::NodeType "NodeType::NodeDeclaration" nodes can only be added as children of the document node; attempt to insert declaration node as a child of an element node fails;
///     - Adding node/attribute results in memory allocation, which may fail;
///     - Insertion functions fail if the specified node or attribute is null or is not in the target node's children/attribute list.
///
/// Even if the operation fails, the document remains in consistent state, but the requested node/attribute is not added.
/// @warning GetAttribute() and GetChild() functions do not add attributes or nodes to the tree, so code like node.GetAttribute("id") = 123; will not do anything if node does not
/// have an attribute with name "id". Make sure you're operating with existing attributes/nodes by adding them if necessary.
///
/// This is an example of adding new attributes/nodes to the document:
/// @code
/// // add node with some name
/// Mezzanine::XML::Node node = doc.AppendChild("node");
///
/// // add description node with text child
/// Mezzanine::XML::Node descr = node.AppendChild("description");
/// descr.AppendChild(Mezzanine::XML::NodePcdata).SetValue("Simple node");
///
/// // add param node before the description
/// Mezzanine::XML::Node param = node.InsertChildBefore("param", descr);
///
/// // add attributes to param node
/// param.AppendAttribute("name") = "version";
/// param.AppendAttribute("value") = 1.1;
/// param.InsertAttributeAfter("type", param.GetAttribute("name")) = "float";
/// @endcode
/// @subsection XMLModifyingRemoving Removing Nodes/Attributes
/// If you do not want your document to contain some node or attribute, you can remove it with one of the following functions:
/// @code
/// bool Mezzanine::XML::Node::RemoveAttribute(const Mezzanine::XML::Attribute& a);
/// bool Mezzanine::XML::Node::RemoveChild(const Mezzanine::XML::Node& n);
/// @endcode
/// RemoveAttribute removes the attribute from the attribute list of the node, and returns the operation result. RemoveChild removes the child node with the entire subtree (including
/// all descendant nodes and attributes) from the document, and returns the operation result. Removing fails if one of the following is true:
///     - The node the function is called on is null;
///     - The attribute/node to be removed is null;
///     - The attribute/node to be removed is not in the node's attribute/child list.
///
/// Removing the attribute or node invalidates all handles to the same underlying object, and also invalidates all iterators pointing to the same object. Removing node also invalidates
/// all past-the-end iterators to its attribute or child node list. Be careful to ensure that all such handles and iterators either do not exist or are not used after the attribute/node
/// is removed.
/// \n \n
/// If you want to remove the attribute or child node by its name, two additional helper functions are available:
/// @code
/// bool Mezzanine::XML::Node::RemoveAttribute(const char_t* Name);
/// bool Mezzanine::XML::Node::RemoveChild(const char_t* Name);
/// @endcode
/// These functions look for the first attribute or child with the specified name, and then remove it, returning the result. If there is no attribute or child with such name, the
/// function returns false; if there are two nodes with the given name, only the first node is deleted. If you want to delete all nodes with the specified name, you can use code
/// like this: while (node.RemoveChild("tool")) ;.
/// \n \n
/// This is an example of removing attributes/nodes from the document:
/// @code
/// // remove description node with the whole subtree
/// Mezzanine::XML::Node node = doc.GetChild("node");
/// node.RemoveChild("description");
///
/// // remove id attribute
/// Mezzanine::XML::Node param = node.GetChild("param");
/// param.RemoveAttribute("value");
///
/// // we can also remove nodes/attributes by handles
/// Mezzanine::XML::Attribute id = param.GetAttribute("name");
/// param.RemoveAttribute(id);
/// @endcode
/// @subsection XMLModifyingCloning Cloning Nodes/Attributes
/// With the help of previously described functions, it is possible to create trees with any contents and structure, including cloning the existing data. However since this is an often
/// needed operation, Mezzanine::xml provides built-in node/attribute cloning facilities. Since nodes and attributes do not exist without a document tree, you can't create a standalone copy
/// - you have to immediately insert it somewhere in the tree. For this, you can use one of the following functions:
/// @code
/// Mezzanine::XML::Attribute Mezzanine::XML::Node::AppendCopy(const Mezzanine::XML::Attribute& proto);
/// Mezzanine::XML::Attribute Mezzanine::XML::Node::PrependCopy(const Mezzanine::XML::Attribute& proto);
/// Mezzanine::XML::Attribute Mezzanine::XML::Node::InsertCopyAfter(const Mezzanine::XML::Attribute& proto, const Mezzanine::XML::Attribute& attr);
/// Mezzanine::XML::Attribute Mezzanine::XML::Node::InsertCopyAfter(const Mezzanine::XML::Attribute& proto, const Mezzanine::XML::Attribute& attr);
///
/// Mezzanine::XML::Node Mezzanine::XML::Node::AppendCopy(const Mezzanine::XML::Node& proto);
/// Mezzanine::XML::Node Mezzanine::XML::Node::PrependCopy(const Mezzanine::XML::Node& proto);
/// Mezzanine::XML::Node Mezzanine::XML::Node::InsertCopyAfter(const Mezzanine::XML::Node& proto, const Mezzanine::XML::Node& node);
/// Mezzanine::XML::Node Mezzanine::XML::Node::InsertCopyAfter(const Mezzanine::XML::Node& proto, const Mezzanine::XML::Node& node);
/// @endcode
///
/// These functions mirror the structure of AppendChild, PrependChild, InsertChildBefore and related functions - they take the handle to the prototype object, which is to be cloned,
/// insert a new attribute/node at the appropriate place, and then copy the attribute data or the whole node subtree to the new object. The functions return the handle to the resulting
/// duplicate object, or null handle on failure.
/// \n \n
/// The attribute is copied along with the name and value; the node is copied along with its type, name and value; additionally attribute list and all children are recursively cloned,
/// resulting in the deep subtree clone. The prototype object can be a part of the same document, or a part of any other document.
/// \n \n
/// The failure conditions resemble those of AppendChild, InsertChild_before and related functions, consult their documentation for more information. There are additional caveats
/// specific to cloning functions:
///     - Cloning null handles results in operation failure;
///     - Node cloning starts with insertion of the node of the same type as that of the prototype; for this reason, cloning functions can not be directly used to clone entire
///     documents, since @ref Mezzanine::XML::NodeType "NodeType::NodeDocument" is not a valid insertion type. The example below provides a workaround.
///     - It is possible to copy a subtree as a child of some node inside this subtree, i.e. node.AppendCopy(node.GetParent().GetParent());. This is a valid operation, and it results
///     in a clone of the subtree in the state before cloning started, i.e. no infinite recursion takes place.
///
/// This is an example with one possible implementation of include tags in XML. It illustrates node cloning and usage of other document modification functions:
/// @code
/// bool LoadPreprocess(Mezzanine::XML::Document& doc, const char* path);
///
/// bool PreProcess(Mezzanine::XML::Node node)
/// {
///     for (Mezzanine::XML::Node child = node.GetFirstChild(); child; )
///     {
///         if (child.Type() == Mezzanine::XML::NodePi && strcmp(child.Name(), "include") == 0)
///         {
///             Mezzanine::XML::Node include = child;
///
///             // load new preprocessed document (note: ideally this should handle relative paths)
///             const char* path = include.Value();
///
///             Mezzanine::XML::Document doc;
///             if (!LoadPreprocess(doc, path)) return false;
///
///             // insert the comment marker above include directive
///             node.InsertChildBefore(pugi::NodeComment, include).SetValue(path);
///
///             // copy the document above the include directive (this retains the original order!)
///             for (Mezzanine::XML::Node ic = doc.FirstChild(); ic; ic = ic.NextSibling())
///             {
///                 node.InsertCopyAfter(ic, include);
///             }
///
///             // remove the include node and move to the next child
///             child = child.NextSibling();
///
///             node.RemoveChild(include);
///         }
///         else
///         {
///             if (!PreProcess(child)) return false;
///
///             child = child.NextSibling();
///         }
///     }
///
///     return true;
/// }
///
/// bool LoadPreprocess(Mezzanine::XML::Document& doc, const char* path)
/// {
///     Mezzanine::XML::ParseResult result = doc.LoadFile(path, Mezzanine::XML::ParseDefault | Mezzanine::XML::ParsePi); // for <?include?>
///
///     return result ? PreProcess(doc) : false;
/// }
/// @endcode
/// @section XMLSaving Saving Documents
///     - @ref XMLSavingFile
///     - @ref XMLSavingStreams
///     - @ref XMLSavingWriter
///     - @ref XMLSavingSubtree
///     - @ref XMLSavingOptions
///     - @ref XMLSavingEncodings
///
/// Often after creating a new document or loading the existing one and processing it, it is necessary to save the result back to file. Also it is occasionally useful to output the
/// whole document or a subtree to some stream; use cases include debug printing, serialization via network or other text-oriented medium, etc. Mezzanine::xml provides several functions
/// to output any subtree of the document to a file, stream or another generic transport interface; these functions allow one to customize the output format ( see @ref XMLSavingOptions ), and
/// also perform necessary encoding conversions ( see @ref XMLSavingEncodings ). This section documents the relevant functionality.
/// \n \n
/// Before writing to the destination the node/attribute data is properly formatted according to the node type; all special XML symbols, such as \< and \&, are properly escaped. In
/// order to guard against forgotten node/attribute names, empty node/attribute names are printed as ":anonymous". For well-formed output, make sure all node and attribute names are
/// set to meaningful values.
/// \n \n
/// CDATA sections with values that contain "]]>" are split into several sections as follows: section with value "pre]]>post" is written as <![CDATA[pre]]]]><![CDATA[>post]]>. While
/// this alters the structure of the document (if you load the document after saving it, there will be two CDATA sections instead of one), this is the only way to escape CDATA contents.
/// @subsection XMLSavingFile Saving Documents to Files
/// If you want to save the whole document to a file, you can use one of the following functions:
/// @code
/// bool Mezzanine::XML::Document::SaveFile(const char* Path, const Mezzanine::XML::char_t* indent = "\t", unsigned int flags = Mezzanine::XML::FormatDefault, Mezzanine::XML::Encoding encoding = EncodingAuto) const;
/// bool Mezzanine::XML::Document::SaveFile(const wchar_t* Path, const Mezzanine::XML::char_t* indent = "\t", unsigned int flags = Mezzanine::XML::FormatDefault, Mezzanine::XML::Encoding encoding = EncodingAuto) const;
/// @endcode
/// These functions accept file path as its first argument, and also three optional arguments, which specify indentation and other output options (see Output options) and output data
/// encoding ( see @ ref XMLSavingEncodings ). The Path has the target operating system format, so it can be a relative or absolute one, it should have the delimiters of the target
/// system, it should have the exact case if the target file system is case-sensitive, etc.
/// \n \n
/// File path is passed to the system file opening function as is in case of the first function (which accepts const char* Path); the second function either uses a special file opening
/// function if it is provided by the runtime library or converts the path to UTF-8 and uses the system file opening function.
/// \n \n
/// SaveFile opens the target file for writing, outputs the requested header (by default a document declaration is output, unless the document already has one), and then saves the
/// document contents. If the file could not be opened, the function returns false. Calling SaveFile is equivalent to creating an Mezzanine::XML::WriterFile object with FILE* handle as the
/// only constructor argument and then calling save; see @ref XMLSavingWriter for writer interface details.
/// \n \n
/// This is a simple example of saving XML document to a file :
/// @code
/// // save document to file
/// std::cout << "Saving result: " << doc.SaveFile("save_file_output.xml") << std::endl;
/// @endcode
/// @subsection XMLSavingStreams Saving Documents to C++ IOstreams
/// To enhance interoperability Mezzanine::xml provides functions for saving document to any object which implements C++ std::ostream interface. This allows you to save documents to any
/// standard C++ stream (i.e. file stream) or any third-party compliant implementation (i.e. Boost Iostreams). Most notably, this allows for easy debug output, since you can use
/// std::cout stream as saving target. There are two functions, one works with narrow character streams, another handles wide character ones:
/// @code
/// void Mezzanine::XML::Document::Save(std::ostream& stream, const char_t* indent = "\t", unsigned int flags = Mezzanine::XML::FormatDefault, Mezzanine::XML::Encoding encoding = EncodingAuto) const;
/// void Mezzanine::XML::Document::Save(std::wostream& stream, const char_t* indent = "\t", unsigned int flags = Mezzanine::XML::FormatDefault) const;
/// @endcode
/// Calling Save with an std::ostream argument saves the document to the stream in the same way as SaveFile (i.e. with requested header and with encoding conversions). On the other
/// hand, save with std::wstream argument saves the document to the wide stream with @ref Mezzanine::XML::Encoding "Encoding::Encodingwchar_t" encoding. Because of this, using save with wide
/// character streams requires careful (usually platform-specific) stream setup (i.e. using the imbue function). Generally use of wide streams is discouraged, however it provides you
/// with the ability to save documents to non-Unicode encodings, i.e. you can save Shift-JIS encoded data if you set the correct locale.
/// \n \n
/// Calling Save with stream target is equivalent to creating an Mezzanine::XML::WriterStream object with stream as the only constructor argument and then calling save; see
/// @ref XMLSavingWriter for writer interface details.
/// \n \n
/// This is a simple example of saving XML document to standard output:
/// @code
/// // save document to standard output
/// std::cout << "Document:\n";
/// doc.Save(std::cout);
/// @endcode
/// @subsection XMLSavingWriter Saving Document via Writer Interface
/// All of the above saving functions are implemented in terms of the Mezzanine::XML::Writer interface. This is a simple interface with a single function, which is called several times
/// during output process with chunks of document data as input.
/// @code
/// class Writer
/// {
/// public:
///     virtual void Write(const void* data, size_t size) = 0;
/// };
///
/// void Mezzanine::XML::Document::Save (Writer &WriterInstance, const char_t *indent="\t", unsigned int flags=FormatDefault, Encoding DocumentEncoding=EncodingAuto) const;
/// @endcode
/// In order to output the document via some custom transport, for example sockets, you should create an object which implements Mezzanine::XML::Writer interface and pass it to save function.
/// Mezzanine::XML::Writer::Write function is called with a buffer as an input, where data points to buffer start, and size is equal to the buffer size in bytes. The Write implementation must
/// write the buffer to the transport; it can not save the passed buffer pointer, as the buffer contents will change after write returns. The buffer contains the chunk of document data
/// in the desired encoding.
/// \n \n
/// The Write function is called with relatively large blocks (size is usually several kilobytes, except for the first block with BOM, which is output only if Mezzanine::XML::FormatWriteBom
/// is set, and last block, which may be small), so there is often no need for additional buffering in the implementation.
/// \n \n
/// This is a simple example of custom writer for saving document data to STL string:
/// @code
/// struct StringWriter: Mezzanine::XML::Writer
/// {
///     std::string Result;
///
///     virtual void Write(const void* data, size_t size)
///     {
///         Result += std::string(static_cast<const char*>(data), size);
///     }
/// };
/// @endcode
/// @subsection XMLSavingSubtree Saving a Single Subtree
/// While the previously described functions save the whole document to the destination, it is easy to save a single subtree. The following functions are provided:
/// @code
/// void Mezzanine::XML::Node::Print(std::basic_ostream< char, std::char_traits< char > > &os, const char_t *indent="\t", unsigned int flags=FormatDefault, Encoding DocumentEncoding=EncodingAuto, unsigned int Depth=0) const ;
/// void Mezzanine::XML::Node::Print(std::basic_ostream< wchar_t, std::char_traits< wchar_t > > &os, const char_t *indent="\t", unsigned int flags=FormatDefault, unsigned int Depth=0) const ) const;
/// void Mezzanine::XML::Node::Print(Mezzanine::XML::Writer &WriterInstance, const char_t *indent="\t", unsigned int flags=FormatDefault, Encoding DocumentEncoding=EncodingAuto, unsigned int Depth=0) const;
/// @endcode
/// These functions have the same arguments with the same meaning as the corresponding Mezzanine::XML::Document::Save functions, and allow you to save the subtree to either a C++ IOstream
/// or to any object that implements Mezzanine::XML::Writer interface.
/// \n \n
/// Saving a subtree differs from saving the whole document: the process behaves as if Mezzanine::XML::FormatWriteBom is off, and Mezzanine::XML::FormatNoDeclaration is on, even if actual values
/// of the flags are different. This means that BOM is not written to the destination, and document declaration is only written if it is the node itself or is one of node's children.
/// Note that this also holds if you're saving a document; this example illustrates the difference:
/// @code
/// // get a test document
/// Mezzanine::XML::Document doc;
/// doc.Load("<foo bar='baz'><call>hey</call></foo>");
///
/// // print document to standard output (prints <?xml version="1.0"?><foo bar="baz"><call>hey</call></foo>)
/// doc.Save(std::cout, "", Mezzanine::XML::FormatRaw);
/// std::cout << std::endl;
///
/// // print document to standard output as a regular node (prints <foo bar="baz"><call>hey</call></foo>)
/// doc.Print(std::cout, "", Mezzanine::XML::FormatRaw);
/// std::cout << std::endl;
///
/// // print a subtree to standard output (prints <call>hey</call>)
/// doc.GetChild("foo").GetChild("call").Print(std::cout, "", Mezzanine::XML::FormatRaw);
/// std::cout << std::endl;
/// @endcode
/// @subsection XMLSavingOptions Output Options
/// All saving functions accept the optional parameter flags. This is a bitmask that customizes the output format; you can select the way the document nodes are printed and select the
/// needed additional information that is output before the document contents.
/// @note You should use the usual bitwise arithmetics to manipulate the bitmask: to enable a flag, use mask | flag; to disable a flag, use mask & ~flag.
///
/// These flags control the resulting tree contents:
///     - Mezzanine::XML::FormatIndent determines if all nodes should be indented with the indentation string (this is an additional parameter for all saving functions, and is "\t" by - default).
///     If this flag is on, before every node the indentation string is output several times, where the amount of indentation depends on the node's depth relative to the
///     output subtree. This flag has no effect if Mezzanine::XML::FormatRaw is enabled. This flag is off by default.
///     - Mezzanine::XML::FormatRaw switches between formatted and raw output. If this flag is on, the nodes are not indented in any way, and also no newlines that are not part of
///     document text are printed. Raw mode can be used for serialization where the result is not intended to be read by humans; also it can be useful if the document was parsed
///     with Mezzanine::XML::ParseWsPcdata flag, to preserve the original document formatting as much as possible. This flag is on by default.
///
///These flags control the additional output information:
///     - Mezzanine::XML::FormatNoDeclaration disables default node declaration output. By default, if the document is saved via save or SaveFile function, and it does not have any
///     document declaration, a default declaration is output before the document contents. Enabling this flag disables this declaration. This flag has no effect in
///     Mezzanine::XML::Node::Print functions: they never output the default declaration. This flag is off by default.
///     - Mezzanine::XML::FormatWriteBom enables Byte Order Mark (BOM) output. By default, no BOM is output, so in case of non UTF-8 encodings the resulting document's encoding may not
///     be recognized by some parsers and text editors, if they do not implement sophisticated encoding detection. Enabling this flag adds an encoding-specific BOM to the output. This
///     flag has no effect in Mezzanine::XML::Node::Print functions: they never output the BOM. This flag is off by default.
///
/// Additionally, there is one predefined option mask:
///     - Mezzanine::XML::FormatDefault is the default set of flags, i.e. it has all options set to their default values. This differs from the default pugixml settings and only sets
///     Mezzanine::XML::FormatRaw . This is to minimize output size, and optimize for computer consumption of xml.
///
/// @subsection XMLSavingEncodings Encodings
/// Mezzanine::xml supports all popular Unicode encodings (UTF-8, UTF-16 (big and little endian), UTF-32 (big and little endian); UCS-2 is naturally supported since it's a strict subset
/// of UTF-16) and handles all encoding conversions during output. The output encoding is set via the encoding parameter of saving functions, which is of type xml_encoding. The
/// possible values for the encoding are documented in Encodings; the only flag that has a different meaning is EncodingAuto.
/// \n \n
/// While all other flags set the exact encoding, EncodingAuto is meant for automatic encoding detection. The automatic detection does not make sense for output encoding, since there
/// is usually nothing to infer the actual encoding from, so here @ref Mezzanine::XML::Encoding "EncodingAuto" means UTF-8 encoding, which is the most popular encoding for XML data storage. This is also the default
/// value of output encoding; specify another value if you do not want UTF-8 encoded output.
/// \n \n
/// Also note that wide stream saving functions do not have encoding argument and always assume @ref Mezzanine::XML::Encoding "Encodingwchar_t" encoding.
/// @note The current behavior for Unicode conversion is to skip all invalid UTF sequences during conversion. This behavior should not be relied upon; if your node/attribute names do
/// not contain any valid UTF sequences, they may be output as if they are empty, which will result in malformed XML document.
/// @section XMLXPath XPath
///     - @ref XMLXPathTypes
///     - @ref XMLXPathSelecting
///     - @ref XMLXPathQuery
///     - @ref XMLXPathVariables
///     - @ref XMLXPathError
///     - @ref XMLXPathStandards
///
/// If the task at hand is to select a subset of document nodes that match some criteria, it is possible to code a function using the existing traversal functionality for any practical
/// criteria. However, often either a data-driven approach is desirable, in case the criteria are not predefined and come from a file, or it is inconvenient to use traversal interfaces
/// and a higher-level Domain Specific Language is required. There is a standard language for XML processing, XPath, that can be useful for these cases. Mezzanine::xml implements an almost
/// complete subset of XPath 1.0. Because of differences in document object model and some performance implications, there are minor violations of the official specifications, which
/// can be found in @ref XMLXPathStandards . The rest of this section describes the interface for XPath functionality. Please note that if you wish to learn to use XPath language, you
/// have to look for other tutorials or manuals.
/// \n \n
/// @subsection XMLXPathTypes XPath Types
/// Each XPath expression can have one of the following types: boolean, number, string or node set. Boole type corresponds to bool type, number type corresponds to double type, string
/// type corresponds to either std::string or std::wstring, depending on whether wide character interface is enabled, and node set corresponds to Mezzanine::XML::XPathNodeSet type. There is
/// an enumeration, Mezzanine::XML::XPathValueType, which can take the values @ref Mezzanine::XML::XPathValueType "XPathTypeBoole", @ref Mezzanine::XML::XPathValueType "XPathTypeNumber",
/// @ref Mezzanine::XML::XPathValueType XPathTypeString or @ref Mezzanine::XML::XPathValueType XPathTypeNodeSet, accordingly.
/// \n \n
/// Because an XPath node can be either a node or an attribute, there is a special type, Mezzanine::XML::XPathNode, which is a discriminated union of these types. A value of this type
/// contains two node handles, one of Mezzanine::XML::Node type, and another one of Mezzanine::XML::Attribute type; at most one of them can be non-null. The accessors to get these handles are
/// available:
/// @code
/// Mezzanine::XML::Node Mezzanine::XML::XPathNode::GetNode() const;
/// Mezzanine::XML::Attribute Mezzanine::XML::XPathNode::GetAttribute() const;
/// @endcode
/// XPath nodes can be null, in which case both accessors return null handles.
/// \n \n
/// Note that as per XPath specification, each XPath node has a parent, which can be retrieved via this function:
/// @code
/// Mezzanine::XML::Node Mezzanine::XML::XPathNode::GetParent() const;
/// @endcode
/// The GetParent function returns the node's parent if the XPath node corresponds to Mezzanine::XML::Node handle (equivalent to GetNode().GetParent()), or the node to which the attribute
/// belongs to, if the XPath node corresponds to Mezzanine::XML::Attribute handle. For null nodes, parent returns null handle.
/// \n \n
/// Like node and attribute handles, XPath node handles can be implicitly cast to boolean-like object to check if it is a null node, and also can be compared for equality with
/// each other.
/// \n \n
/// You can also create XPath nodes with one of the three constructors: the default constructor, the constructor that takes node argument, and the constructor that takes an attribute
/// and a node argument (in which case the attribute must belong to the attribute list of the node). The constructor from Mezzanine::XML::Node is implicit, so you can usually pass
/// Mezzanine::XML::Node to functions that expect Mezzanine::XML::XPathNode. Apart from that you usually don't need to create your own XPath node objects, since they are returned to you via
/// selection functions.
/// \n \n
/// XPath expressions operate not on single nodes, but instead on node sets. A node set is a collection of nodes, which can be optionally ordered in either a forward document order
/// or a reverse one. Document order is defined in XPath specification; an XPath node is before another node in document order if it appears before it in XML representation of the
/// corresponding document.
/// \n \n
/// Node sets are represented by Mezzanine::XML::XPathNodeSet object, which has an interface that resembles one of sequential random-access containers. It has an iterator type along with
/// usual begin/past-the-end iterator accessors:
/// @code
/// typedef const Mezzanine::XML::XPathNode* Mezzanine::XML::XPathNodeSet::const_iterator;
/// const_iterator Mezzanine::XML::XPathNodeSet::begin() const;
/// const_iterator Mezzanine::XML::XPathNodeSet::end() const;
/// @endcode
/// And it also can be iterated via indices, just like std::vector:
/// @code
/// const Mezzanine::XML::XPathNode& Mezzanine::XML::XPathNodeSet::operator[](size_t index) const;
/// size_t Mezzanine::XML::XPathNodeSet::size() const;
/// bool Mezzanine::XML::XPathNodeSet::empty() const;
/// @endcode
/// All of the above operations have the same semantics as that of std::vector: the iterators are random-access, all of the above operations are constant time, and accessing the element
/// at index that is greater or equal than the set size results in undefined behavior. You can use both iterator-based and index-based access for iteration, however the iterator-based
/// one can be faster.
/// \n \n
/// The order of iteration depends on the order of nodes inside the set; the order can be queried via the following function:
/// @code
/// enum Mezzanine::XML::XPathNodeSet::CollectionType { TypeUnsorted, TypeSorted, TypeSortedReverse };
/// CollectionType Mezzanine::XML::XPathNodeSet::Type() const;
/// @endcode
/// The Type function returns the current order of nodes; TypeSorted means that the nodes are in forward document order, TypeSortedReverse means that the nodes are in reverse document
/// order, and TypeUnsorted means that neither order is guaranteed (nodes can accidentally be in a sorted order even if Type() returns TypeUnsorted). If you require a specific order of
/// iteration, you can change it via sort function:
/// void Mezzanine::XML::XPathNodeSet::sort(bool reverse = false);
/// @warning When converted from the pugixml naming scheme, changes were made to this class conservatively. Once it is determine what the new names should be. this could be changed.
///
/// Calling sort sorts the nodes in either forward or reverse document order, depending on the argument; after this call Type() will return TypeSorted or TypeSortedReverse.
/// \n \n
/// Often the actual iteration is not needed; instead, only the first element in document order is required. For this, a special accessor is provided:
/// @code
/// Mezzanine::XML::XPathNode Mezzanine::XML::XPathNodeSet::first() const;
/// @endcode
/// This function returns the first node in forward document order from the set, or null node if the set is empty. Note that while the result of the node does not depend on the order of
/// nodes in the set (i.e. on the result of Type()), the complexity does - if the set is sorted, the complexity is constant, otherwise it is linear in the number of elements or worse.
/// \n \n
/// While in the majority of cases the node set is returned by XPath functions, sometimes there is a need to manually construct a node set. For such cases, a constructor is provided
/// which takes an iterator range (const_iterator is a typedef for const Mezzanine::XML::XPathNode*), and an optional type:
/// @code
/// Mezzanine::XML::XPathNodeSet::XPathNodeSet(const_iterator begin, const_iterator end, CollectionType Type=TypeUnsorted);
/// @endcode
/// The constructor copies the specified range and sets the specified type. The objects in the range are not checked in any way; you'll have to ensure that the range contains no
/// duplicates, and that the objects are sorted according to the type parameter. Otherwise XPath operations with this set may produce unexpected results.
/// \n \n
/// @subsection XMLXPathSelecting Selecting Nodes via XPath Expression
/// If you want to select nodes that match some XPath expression, you can do it with the following functions:
/// @code
/// Mezzanine::XML::XPathNode Mezzanine::XML::Node::FindSingleNode(const char_t* query, XPathVariableSet* variables = 0) const;
/// Mezzanine::XML::XPathNodeSet Mezzanine::XML::Node::FindNodes(const char_t* query, XPathVariableSet* variables = 0) const;
/// @endcode
/// The FindNodes function compiles the expression and then executes it with the node as a context node, and returns the resulting node set. FindSingleNode returns only the first node
/// in document order from the result, and is equivalent to calling FindNodes(query).first(). If the XPath expression does not match anything, or the node handle is null, FindNodes
/// returns an empty set, and FindSingleNode returns null XPath node.
/// \n \n
/// Both functions throw Mezzanine::XML::XPathException if the query can not be compiled or if it returns a value with type other than node set; see
/// Error handling for details.
/// \n \n
/// While compiling expressions is fast, the compilation time can introduce a significant overhead if the same expression is used many times on small subtrees. If you're doing many
/// similar queries, consider compiling them into query objects (see @ref XMLXPathQuery for further reference). Once you get a compiled query object, you can pass it to Find functions
/// instead of an expression string:
/// @code
/// Mezzanine::XML::XPathNode Mezzanine::XML::Node::FindSingleNode(const Mezzanine::XML::XPathQuery& query) const;
/// Mezzanine::XML::XPathNodeSet Mezzanine::XML::Node::FindNodes(const Mezzanine::XML::XPathQuery& query) const;
/// @endcode
/// Both functions throw Mezzanine::XML::XPathException if the query returns a value with type other than node set.
/// \n \n
/// This is an example of selecting nodes using XPath expressions:
/// @code
/// Mezzanine::XML::XPathNodeSet tools = doc.FindNodes("/Profile/Tools/Tool[@AllowRemote='true' and @DeriveCaptionFrom='lastparam']");
///
/// std::cout << "Tools:";
///
/// for (Mezzanine::XML::XPathNodeSet::const_iterator it = tools.begin(); it != tools.end(); ++it)
/// {
///     Mezzanine::XML::XPathNode node = *it;
///     std::cout << " " << node.GetNode().GetAttribute("Filename").Value();
/// }
///
/// Mezzanine::XML::XPathNode BuildTool = doc.FindSingleNode("//Tool[contains(Description, 'build system')]");
///
/// std::cout << "\nBuild tool: " << BuildTool.GetNode().GetAttribute("Filename").Value() << "\n";
/// @endcode
/// @subsection XMLXPathQuery Using Query Objects
/// When you call FindNodes with an expression string as an argument, a query object is created behind the scenes. A query object represents a compiled XPath expression. Query objects
/// can be useful in the following circumstances:
///     - You can precompile expressions to query objects to save compilation time if it becomes an issue;
///     - You can use query objects to evaluate XPath expressions which result in booleans, numbers or strings;
///     - You can get the type of expression value via query object.
///
/// Query objects correspond to Mezzanine::XML::XPathQuery type. They are immutable and non-copyable: they are bound to the expression at creation time and can not be cloned. If you want to
/// put query objects in a container, allocate them on heap via new operator and store pointers to Mezzanine::XML::XPathQuery in the container.
/// \n \n
/// You can create a query object with the constructor that takes XPath expression as an argument:
/// @code
/// explicit Mezzanine::XML::XPathQuery::XPathQuery(const char_t* query, XPathVariableSet* variables = 0);
/// @endcode
/// The expression is compiled and the compiled representation is stored in the new query object. If compilation fails, XPathException is thrown if exception handling is not disabled
/// (see @ref XMLXPathError for details). After the query is created, you can query the type of the evaluation result using the following function:
/// @code
/// Mezzanine::XML::XPathValueType Mezzanine::XML::XPathQuery::ReturnType() const;
/// @endcode
/// You can evaluate the query using one of the following functions:
/// @code
/// bool Mezzanine::XML::XPathQuery::EvaluateBoole(const Mezzanine::XML::XPathNode& n) const;
/// double Mezzanine::XML::XPathQuery::EvaluateNumber(const Mezzanine::XML::XPathNode& n) const;
/// Mezzanine::String Mezzanine::XML::XPathQuery::EvaluateString(const Mezzanine::XML::XPathNode& n) const;
/// Mezzanine::XML::XPathNodeSet Mezzanine::XML::XPathQuery::EvaluateNodeSet(const Mezzanine::XML::XPathNode& n) const;
/// @endcode
/// All functions take the context node as an argument, compute the expression and return the result, converted to the requested type. According to XPath specification, value of any
/// type can be converted to boolean, number or string value, but no type other than node set can be converted to node set. Because of this, EvaluateBoole, EvaluateNumber and
/// EvaluateString always return a result, but EvaluateNodeSet results in an error if the return type is not node set ( see @ref XMLXPathError ).
/// @note Calling node.FindNodes("query") is equivalent to calling Mezzanine::XML::XPathQuery("query").EvaluateNodeSet(node).
///
/// There is another string evaluation
/// function:
/// @code
/// size_t Mezzanine::XML::XPathQuery::EvaluateString(char_t* buffer, size_t capacity, const Mezzanine::XML::XPathNode& n) const;
/// @endcode
/// This function evaluates the string, and then writes the result to buffer (but at most capacity characters); then it returns the full size of the result in characters, including
/// the terminating zero. If capacity is not 0, the resulting buffer is always zero-terminated. You can use this function as follows:
///     - First call the function with buffer = 0 and capacity = 0; then allocate the returned amount of characters, and call the function again, passing the allocated storage and the
///     amount of characters;
///     - First call the function with small buffer and buffer capacity; then, if the result is larger than the capacity, the output has been trimmed, so allocate a larger buffer and
///     call the function again.
///
/// This is an example of using query objects:
/// @code
/// // Select nodes via compiled query
/// Mezzanine::XML::XPathQuery QueryRemoteTools("/Profile/Tools/Tool[@AllowRemote='true']");
///
/// Mezzanine::XML::XPathNodeSet tools = QueryRemoteTools.EvaluateNodeSet(doc);
/// std::cout << "Remote tool: ";
/// tools[2].node().print(std::cout);
///
/// // Evaluate numbers via compiled query
/// Mezzanine::XML::XPathQuery QueryTimeouts("sum(//Tool/@Timeout)");
/// std::cout << QueryTimeouts.EvaluateNumber(doc) << std::endl;
///
/// // Evaluate strings via compiled query for different context nodes
/// Mezzanine::XML::XPathQuery QueryNameValid("string-length(substring-before(@Filename, '_')) > 0 and @OutputFileMasks");
/// Mezzanine::XML::XPathQuery QueryName("concat(substring-before(@Filename, '_'), ' produces ', @OutputFileMasks)");
///
/// for (Mezzanine::XML::Node tool = doc.FirstElementByPath("Profile/Tools/Tool"); tool; tool = tool.NextSibling())
/// {
///     std::string s = QueryName.EvaluateString(tool);
///
///     if (QueryNameValid.EvaluateBoole(tool)) std::cout << s << std::endl;
/// }
/// @endcode
/// @subsection XMLXPathVariables Using Variables
/// XPath queries may contain references to variables; this is useful if you want to use queries that depend on some dynamic parameter without manually preparing the complete query
/// string, or if you want to reuse the same query object for similar queries.
/// \n \n
/// Variable references have the form $name; in order to use them, you have to provide a variable set, which includes all variables present in the query with correct types. This set
/// is passed to Mezzanine::XML::XPathQuery constructor or to FindNodes/FindNode functions:
/// @code
/// explicit Mezzanine::XML::XPathQuery::Mezzanine::XML::XPathQuery(const char_t* query, XPathVariableSet* variables = 0);
/// Mezzanine::XML::XPathNode xml_node::FindNode(const char_t* query, XPathVariableSet* variables = 0) const;
/// XPathNodeSet xml_node::FindNodes(const char_t* query, XPathVariableSet* variables = 0) const;
/// @endcode
/// If you're using query objects, you can change the variable values before evaluate/select calls to change the query behavior.
/// @note The variable set pointer is stored in the query object; you have to ensure that the lifetime of the set exceeds that of query object.
///
/// Variable sets correspond to XPathVariableSet type, which is essentially a variable container.
/// \n \n
/// You can add new variables with the following function:
/// @code
/// Mezzanine::XML::XPathVariable* Mezzanine::XML::XPathVariableSet::Add(const char_t* Name, Mezzanine::XML::XPathValueType type);
/// @endcode
/// The function tries to add a new variable with the specified name and type; if the variable with such name does not exist in the set, the function adds a new variable and returns
/// the variable handle; if there is already a variable with the specified name, the function returns the variable handle if variable has the specified type. Otherwise the function
/// returns null pointer; it also returns null pointer on allocation failure.
/// \n \n
/// New variables are assigned the default value which depends on the type: 0 for numbers, false for booleans, empty string for strings and empty set for node sets.
/// \n \n
/// You can get the existing variables with the following functions:
/// @code
/// XPathVariable* XPathVariableSet::Get(const char_t* Name);
/// const XPathVariable* XPathVariableSet::Get(const char_t* Name) const;
/// @endcode
/// The functions return the variable handle, or null pointer if the variable with the specified name is not found.
/// \n \n
/// Additionally, there are the helper functions for setting the variable value by name; they try to add the variable with the corresponding type, if it does not exist, and to set the
/// value. If the variable with the same name but with different type is already present, they return false; they also return false on allocation failure. Note that these functions do
/// not perform any type conversions.
/// @code
/// bool XPathVariableSet::Set(const char_t* Name, bool Value);
/// bool XPathVariableSet::Set(const char_t* Name, double Value);
/// bool XPathVariableSet::Set(const char_t* Name, const char_t* Value);
/// bool XPathVariableSet::Set(const char_t* Name, const XPathNodeSet& Value);
/// @endcode
/// The variable values are copied to the internal variable storage, so you can modify or destroy them after the functions return.
/// \n \n
/// If setting variables by name is not efficient enough, or if you have to inspect variable information or get variable values, you can use variable handles. A variable corresponds to
/// the XPathVariable type, and a variable handle is simply a pointer to XPathVariable.
/// \n \n
/// In order to get variable information, you can use one of the following functions:
/// @code
/// const char_t* Mezzanine::XML::XPathVariable::Name() const;
/// Mezzanine::XML::XPathValueType Mezzanine::XML::XPathVariable::Type() const;
/// @endcode
/// Note that each variable has a distinct type which is specified upon variable creation and can not be changed later.
/// \n \n
/// In order to get variable value, you should use one of the following functions, depending on the variable type:
/// @code
/// bool Mezzanine::XML::XPathVariable::GetBoole() const;
/// double Mezzanine::XML::XPathVariable::GetNumber() const;
/// const char_t* Mezzanine::XML::XPathVariable::GetString() const;
/// const Mezzanine::XML::XPathNodeSet& Mezzanine::XML::XPathVariable::GetNodeSet() const;
/// @endcode
/// These functions return the value of the variable. Note that no type conversions are performed; if the type mismatch occurs, a dummy value is returned (false for booleans, NaN for
/// numbers, empty string for strings and empty set for node sets).
/// \n \n
/// In order to set variable value, you should use one of the following functions, depending on the variable type:
/// @code
/// bool XPathVariable::Set(bool Value);
/// bool XPathVariable::Set(double Value);
/// bool XPathVariable::Set(const char_t* Value);
/// bool XPathVariable::Set(const XPathNodeSet& Value);
/// @endcode
/// These functions modify the variable value. Note that no type conversions are performed; if the type mismatch occurs, the functions return false; they also return false on allocation
/// failure. The variable values are copied to the internal variable storage, so you can modify or destroy them after the functions return.
/// \n \n
/// This is an example of using variables in XPath queries:
/// @code
/// // Select nodes via compiled query
/// Mezzanine::XML::XPathVariableSet vars;
/// vars.Add("remote", Mezzanine::XML::XPathTypeBoole);
///
/// Mezzanine::XML::XPathQuery QueryRemoteTools("/Profile/Tools/Tool[@AllowRemote = string($remote)]", &vars);
///
/// vars.Set("remote", true);
/// XPathNodeSet ToolsRemote = QueryRemoteTools.EvaluateNodeSet(doc);
///
/// vars.Set("remote", false);
/// Mezzanine::XML::XPathNodeSet ToolsLocal = QueryRemoteTools.EvaluateNodeSet(doc);
///
/// std::cout << "Remote tool: ";
/// ToolsRemote[2].GetNode().Print(std::cout);
///
/// std::cout << "Local tool: ";
/// ToolsLocal[0].GetNode().Print(std::cout);
///
/// // You can pass the context directly to FindNodes/FindNode
/// Mezzanine::XML::XPathNodeSet ToolsLocalImm = doc.FindNodes("/Profile/Tools/Tool[@AllowRemote = string($remote)]", &vars);
///
/// std::cout << "Local tool imm: ";
/// ToolsLocalImm[0].GetNode().Print(std::cout);
/// @endcode
/// @subsection XMLXPathError Error Handling
/// There are two different mechanisms for error handling in XPath implementation; the mechanism used depends on whether exception support is disabled (this is controlled with
/// XML_NO_EXCEPTIONS define).
/// \n \n
/// By default, XPath functions throw Mezzanine::XML::XPathException object in case of errors; additionally, in the event any memory allocation fails, an std::bad_alloc exception is thrown. Also
/// Mezzanine::XML::XPathException is thrown if the query is evaluated to a node set, but the return type is not node set. If the query constructor succeeds (i.e. no exception is thrown), the query
/// object is valid. Otherwise you can get the error details via one of the following functions:
/// @code
/// virtual const char* Mezzanine::XML::XPathException::what() const throw();
/// const Mezzanine::XML::XPathParseResult& Mezzanine::XML::XPathException::Result() const;
/// @endcode
/// If exceptions are disabled, then in the event of parsing failure the query is initialized to invalid state; you can test if the query object is valid by using it in a boolean
/// expression: if (query) { ... }. Additionally, you can get parsing result via the Result() accessor:
/// @code
/// const XPathParseResult& XPathQuery::Result() const;
/// @endcode
/// Without exceptions, evaluating invalid query results in false, empty string, NaN or an empty node set, depending on the type; evaluating a query as a node set results in an empty
/// node set if the return type is not node set.
/// \n \n
/// The information about parsing result is returned via Mezzanine::XML::XPathParseResult object. It contains parsing status and the offset of last successfully parsed character from the
/// beginning of the source stream.
/// \n \n
/// Parsing result is represented as the error message; it is either a null pointer, in case there is no error, or the error message in the form of ASCII zero-terminated string.
/// \n \n
/// The Description() member function can be used to get the error message; it never returns the null pointer, so you can safely use Description() even if query parsing succeeded.
/// \n \n
/// In addition to the error message, parsing result has an OffSet member, which contains the offset of last successfully parsed character. This offset is in units of Mezzanine::XML::char_t
/// (bytes for character mode, wide characters for wide character mode).
/// \n \n
/// Parsing result object can be implicitly converted to bool like this: if (result) { ... } else { ... }.
/// \n \n
/// This is an example of XPath error handling:
/// @code
/// // Exception is thrown for incorrect query syntax
/// try
/// {
///     doc.FindNodes("//nodes[#true()]");
/// }
/// catch (const Mezzanine::XML::XPathException& e)
/// {
///     std::cout << "Select failed: " << e.what() << std::endl;
/// }
///
/// // Exception is thrown for incorrect query semantics
/// try
/// {
/// doc.FindNodes("(123)/next");
/// }
/// catch (const Mezzanine::XML::XPathException& e)
/// {
///     std::cout << "Select failed: " << e.what() << std::endl;
/// }
///
/// // Exception is thrown for query with incorrect return type
/// try
/// {
///     doc.FindNodes("123");
/// }
/// catch (const Mezzanine::XML::XPathException& e)
/// {
///     std::cout << "Select failed: " << e.what() << std::endl;
/// }
/// @endcode
/// @subsection XMLXPathStandards Conformance to W3C Specification
/// Because of the differences in document object models, performance considerations and implementation complexity, pugixml does not provide a fully conformant XPath 1.0 implementation.
/// This is the current list of incompatibilities:
///     - Consecutive text nodes sharing the same parent are not merged, i.e. in \<node>text1 \<![CDATA[data]]> text2 \</node> node should have one text node children, but instead has
///     three.
///     - Since the document type declaration is not used for parsing, id() function always returns an empty node set.
///     - Namespace nodes are not supported (affects namespace:: axis).
///     - Name tests are performed on QNames in XML document instead of expanded names; for \<foo xmlns:ns1='uri' xmlns:ns2='uri'> \<ns1:child/> \<ns2:child/> \</foo>, query foo/ns1:*
///     will return only the first child, not both of them. Compliant XPath implementations can return both nodes if the user provides appropriate namespace declarations.
///     - String functions consider a character to be either a single char value or a single wchar_t value, depending on the library configuration; this means that some string functions
///     are not fully Unicode-aware. This affects substring(), string-length() and translate() functions.



/**
@page OriginalpugixmlLicense Original pugixml License
The original pugixml software had the following license text:
@section pugixmloriginallicense Original Text
@verbatim
The pugixml library is distributed under the MIT license:

     Copyright (c) 2006-2010 Arseny Kapoulkine

     Permission is hereby granted, free of charge, to any person obtaining a copy of this software
     and associated documentation files (the "Software"), to deal in the Software without restriction,
     including without limitation the rights to use, copy, modify, merge, publish, distribute,
     sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
     furnished to do so, subject to the following conditions:

     The above copyright notice and this permission notice shall be included in all copies or
     substantial portions of the Software.

     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
     BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
     NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
     DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This means that you can freely use pugixml in your applications, both open-source and proprietary.
If you use pugixml in a product, it is sufficient to add an acknowledgment like this to the product
distribution:

     This software is based on pugixml library (http://pugixml.org).
     pugixml is Copyright (C) 2006-2010 Arseny Kapoulkine.
@endverbatim
\n \n
The documentation was copied with permission per the copyright notice near the bottom of Table of contents and several other pages as follows:
\n \n
@verbatim
Copyright © 2010 Arseny Kapoulkine
Distributed under the MIT License
@endverbatim
*/

#endif

/*
 * Copyright © 2006-2012 Arseny Kapoulkine
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
