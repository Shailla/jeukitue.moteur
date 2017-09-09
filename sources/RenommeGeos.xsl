<?xml version="1.0" ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="2.0"
				xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
	<xsl:output method="xml" indent="yes" />

	<!-- IdentityTransform -->
	<xsl:template match="@*|node()">
		<xsl:copy>
			<xsl:apply-templates select="@*|node()" />
		</xsl:copy>
	</xsl:template>

	<xsl:template match="/Map">
		<xsl:text>&#xa;</xsl:text>
		<Map xmlns="http://jkt/map2017.xsd">
			<xsl:attribute name="xsi:schemaLocation" namespace="http://www.w3.org/2001/XMLSchema-instance">http://jkt/map2017.xsd H:/jkt/git/moteur/sources/map2017.xsd</xsl:attribute>
			<xsl:apply-templates select="@*|node()" />
		</Map>
	</xsl:template>
	
	<xsl:template match="/Map/Lumieres">
		<Lights>
			<xsl:apply-templates select="@*|node()" />
		</Lights>
	</xsl:template>

	<xsl:template match="/Map/Geos/Geo[@Type='Simple']">
		<GeoSimple>
			<xsl:apply-templates select="@*[name()!='Type']" />
			<xsl:apply-templates select="Solide" />
			<xsl:apply-templates select="Couleur" />
			<xsl:apply-templates select="Sommets" />
			<xsl:apply-templates select="Faces" />
		</GeoSimple>
	</xsl:template>

	<xsl:template match="/Map/Geos/Geo[@Type='Texture']">
		<GeoTexture>
			<xsl:apply-templates select="@*[name()!='Type']" />
			<xsl:apply-templates select="Solide" />
 			<xsl:apply-templates select="Materiau" />
			<xsl:apply-templates select="Sommets" />
			<xsl:apply-templates select="VecteursNormaux" />
			<xsl:apply-templates select="SommetsDeTexture" />
		</GeoTexture>
	</xsl:template>

	<xsl:template match="/Map/Geos/Geo[@Type='SimpleMaterial']">
		<GeoSimpleMaterial>
			<xsl:apply-templates select="@*[name()!='Type']" />
			<xsl:apply-templates select="Solide" />
			<xsl:apply-templates select="Materiau" />
			<xsl:apply-templates select="Sommets" />
			<xsl:apply-templates select="VecteursNormaux" />
		</GeoSimpleMaterial>
	</xsl:template>

	<xsl:template match="/Map/Geos/Geo[@Type='Multi']">
		<GeoMulti>
			<xsl:apply-templates select="@*[name()!='Type']" />
			<xsl:apply-templates select="Solide" />
			<xsl:apply-templates select="Materiau" />
			<xsl:apply-templates select="Sommets" />
			<xsl:apply-templates select="VecteursNormaux" />
			<xsl:apply-templates select="SommetsDeTexture" />
			<xsl:apply-templates select="SousMateriaux" />
		</GeoMulti>
	</xsl:template>

</xsl:stylesheet>