let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

let httpLink =
  ApolloLinks.createHttpLink(~uri="https://fakerql.com/graphql", ());

let client =
  ReasonApollo.createApolloClient(~link=httpLink, ~cache=inMemoryCache, ());

ReactDOMRe.renderToElementWithId(
    <ReasonApollo.Provider client>
        <App />
    </ReasonApollo.Provider>, "app");
