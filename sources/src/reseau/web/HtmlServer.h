/*
 * HtmlServer.h
 *
 *  Created on: 25 déc. 2015
 *      Author: VGDJ7997
 */

#ifndef SRC_RESEAU_WEB_HTMLSERVER_H_
#define SRC_RESEAU_WEB_HTMLSERVER_H_

namespace JktNet
{

class HtmlServer {
public:
	HtmlServer();
	virtual ~HtmlServer();

	void open();
};

}	// JktNet

#endif /* SRC_RESEAU_WEB_HTMLSERVER_H_ */
